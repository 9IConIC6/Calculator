#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <utility>

void drawHUD(const std::vector<double>& results, const std::vector<std::string>& expressions) {
    const int width = 50;
    const int height = 10;
    char screen[height][width + 1];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j)
            screen[i][j] = ' ';
        screen[i][width] = '\0';
    }

    for (int i = 0; i < width; ++i)
        screen[0][i] = screen[height - 1][i] = '-';
    for (int i = 0; i < height; ++i)
        screen[i][0] = screen[i][width - 1] = '|';

    std::string title = " CALCULATOR HUD ";
    for (size_t i = 0; i < title.size() && i + 2 < width - 1; ++i)
        screen[1][2 + i] = title[i];

    if (!results.empty()) {
        std::ostringstream oss;
        oss << "Last result: " << results.back();
        std::string resStr = oss.str();
        for (size_t i = 0; i < resStr.size() && i + 2 < width - 1; ++i)
            screen[3][2 + i] = resStr[i];
    }

    if (!expressions.empty()) {
        std::ostringstream oss;
        oss << "Expr: " << expressions.back();
        std::string exprStr = oss.str();
        for (size_t i = 0; i < exprStr.size() && i + 2 < width - 1; ++i)
            screen[4][2 + i] = exprStr[i];
    }

    std::string btns = "[V] View All  [Y] Continue  [N] Exit";
    for (size_t i = 0; i < btns.size() && i + 2 < width - 1; ++i)
        screen[6][2 + i] = btns[i];

    std::cout << "\n";
    for (int i = 0; i < height; ++i)
        std::cout << screen[i] << "\n";
    std::cout << "\n";
}

void viewAll(const std::vector<double>& results, const std::vector<std::string>& expressions) {
    std::cout << "\n==================== Stored Results ====================\n";
    for (size_t i = 0; i < results.size(); ++i)
        std::cout << std::setw(2) << i + 1 << ": " << results[i] << "\n";

    std::cout << "\n================== Stored Expressions ==================\n";
    for (size_t i = 0; i < expressions.size(); ++i)
        std::cout << std::setw(2) << i + 1 << ": " << expressions[i] << "\n";

    std::cout << "========================================================\n\n";
}

char getCharInput(const std::string& message, const std::vector<double>& results, const std::vector<std::string>& expressions) {
    std::string input;
    while (true) {
        std::cout << message << " ";
        std::cin >> input;

        if (input.length() == 1) {
            char c = std::tolower(input[0]);
            if (c == 'v') {
                viewAll(results, expressions);
                drawHUD(results, expressions);
            } else {
                return c;
            }
        } else {
            std::cout << "Invalid input. Try again.\n";
        }
    }
}

std::pair<std::vector<double>, std::vector<char> > getMultipleNumbersAndOps(
    const std::vector<double>& results,
    const std::vector<std::string>& expressions) {

    std::vector<double> numbers;
    std::vector<char> operations;
    std::string input;

    std::cout << "Enter numbers and operations (type 'done' to finish):\n";

    while (true) {
        std::cout << "Number " << numbers.size() + 1 << ": ";
        std::cin >> input;

        if (input == "done") {
            if (numbers.size() >= 2 && operations.size() == numbers.size() - 1) {
                break;
            } else {
                std::cout << "Please enter at least two numbers and the correct number of operations.\n";
                continue;
            }
        } else if (input[0] == '#' && input.size() > 1) {
            int index;
            std::istringstream(input.substr(1)) >> index;
            if (index >= 1 && index <= static_cast<int>(results.size())) {
                numbers.push_back(results[index - 1]);
            } else {
                std::cout << "Invalid result index.\n";
                continue;
            }
        } else {
            std::stringstream ss(input);
            double value;
            if (ss >> value) {
                numbers.push_back(value);
            } else {
                std::cout << "Invalid number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        }

        if (numbers.size() >= 2) {
            char op = getCharInput("Enter operation (+, -, *, /) for this number", results, expressions);
            if (op == '+' || op == '-' || op == '*' || op == '/') {
                operations.push_back(op);
            } else {
                std::cout << "Invalid operation. Removing last number.\n";
                numbers.pop_back();
            }
        }
    }

    return std::pair<std::vector<double>, std::vector<char> >(numbers, operations);
}

int main() {
    std::vector<double> results;
    std::vector<std::string> expressions;
    char cont = 'y';

    std::cout << "Launching Calculator...\n";
    drawHUD(results, expressions);

    while (std::tolower(cont) == 'y') {
        std::pair<std::vector<double>, std::vector<char> > input = getMultipleNumbersAndOps(results, expressions);
        std::vector<double> numbers = input.first;
        std::vector<char> operations = input.second;

        double result = numbers[0];
        std::ostringstream expr;
        expr << numbers[0];

        bool valid = true;

        for (size_t i = 1; i < numbers.size(); ++i) {
            expr << " " << operations[i - 1] << " " << numbers[i];
            char op = operations[i - 1];
            if (op == '+') result += numbers[i];
            else if (op == '-') result -= numbers[i];
            else if (op == '*') result *= numbers[i];
            else if (op == '/') {
                if (numbers[i] == 0) {
                    std::cout << "Error: Division by zero.\n";
                    valid = false;
                    break;
                } else {
                    result /= numbers[i];
                }
            } else {
                std::cout << "Invalid operation encountered.\n";
                valid = false;
                break;
            }
        }

        if (valid) {
            expr << " = " << result;
            results.push_back(result);
            expressions.push_back(expr.str());
            drawHUD(results, expressions);
            std::cout << "Result: " << result << "\n";
        }

        cont = getCharInput("\nDo you want to continue? (Y/N):", results, expressions);
    }

    std::cout << "Exiting...\n";
    return 0;
}
