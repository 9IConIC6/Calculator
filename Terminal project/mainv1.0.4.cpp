#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cctype>
#include <utility>

const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;

void drawHUD(const std::vector<double>& results, const std::vector<std::string>& expressions) {
    const int width = 55;
    const int height = 12;
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

    std::string title = " ENHANCED CALCULATOR HUD ";
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

    std::string constants = "Constants: [e] 2.71828  [p] 3.14159";
    for (size_t i = 0; i < constants.size() && i + 2 < width - 1; ++i)
        screen[6][2 + i] = constants[i];

    std::string btns = "[V] View All  [Y] Continue  [N] Exit";
    for (size_t i = 0; i < btns.size() && i + 2 < width - 1; ++i)
        screen[8][2 + i] = btns[i];

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
    
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string getOperationInput(const std::vector<double>& results, const std::vector<std::string>& expressions) {
    std::string op;
    while (true) {
        std::cout << "Enter operation (+, -, *, /, ^2, ^, r, root, sin, cos, tan, ln, log), 'v' to view all, or 'done' to finish: ";
        std::cin >> op;
        for (std::size_t i = 0; i < op.size(); ++i)
            op[i] = std::tolower(op[i]);

        if (op == "v") {
            viewAll(results, expressions);
            continue;
        } else if (op == "done" || op == "d") {
            return "done";
        } else if (op == "+" || op == "-" || op == "*" || op == "/" ||
                   op == "^" || op == "^2" || op == "r" || op == "root" ||
                   op == "sin" || op == "cos" || op == "tan" ||
                   op == "ln" || op == "log") {
            return op;
        } else {
            std::cout << "Invalid operation. Try again.\n";
        }
    }
}

bool parseNumber(const std::string& input, double& result, const std::vector<double>& results) {
    if (input.empty()) return false;
    
    if (input == "e" || input == "E") {
        result = E;
        std::cout << "Using constant: e = " << E << "\n";
        return true;
    } else if (input == "p" || input == "pi" || input == "p") {
        result = PI;
        std::cout << "Using constant: p = " << PI << "\n";
        return true;
    }
    else if (input[0] == '#' && input.size() > 1) {
        int index;
        std::istringstream(input.substr(1)) >> index;
        if (index >= 1 && index <= static_cast<int>(results.size())) {
            result = results[index - 1];
            std::cout << "Using stored value #" << index << ": " << result << "\n";
            return true;
        } else {
            std::cout << "Invalid result index.\n";
            return false;
        }
    }
    else {
        std::stringstream ss(input);
        if (ss >> result) {
            return true;
        }
    }
    return false;
}

std::pair<std::vector<double>, std::vector<std::string> > getMultipleNumbersAndOps(
    const std::vector<double>& results,
    const std::vector<std::string>& expressions) {

    std::vector<double> numbers;
    std::vector<std::string> operations;
    std::string input;

    while (true) {
        std::cout << "Number " << numbers.size() + 1 << " (or 'done' to finish, 'v' to view all): ";
        std::cin >> input;

        if (input == "done") {
            if (numbers.size() >= 1) {
                break;
            } else {
                std::cout << "Please enter at least one number.\n";
                continue;
            }
        } else if (input == "v") {
            viewAll(results, expressions);
            continue;
        }

        double value;
        if (parseNumber(input, value, results)) {
            numbers.push_back(value);
        } else {
            std::cout << "Invalid input. Please enter a number, 'e', 'pi', or #n for stored results.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (numbers.size() > 0) {
            std::string op = getOperationInput(results, expressions);
            if (op.empty()) {
                continue;
            } else if (op == "done") {
                break;
            }
            operations.push_back(op);

            if (op == "^2" || op == "r" || op == "sin" || op == "cos" || 
                op == "tan" || op == "ln" || op == "log") {
                break;
            }
        }
    }

    return std::make_pair(numbers, operations);
}

int main() {
    std::vector<double> results;
    std::vector<std::string> expressions;
    char cont = 'y';

    std::cout << "Launching Enhanced Calculator...\n";
    std::cout << "Type 'e' for Euler's number or 'p'/'pi' for p\n";
    drawHUD(results, expressions);

    while (std::tolower(cont) == 'y') {
        std::pair<std::vector<double>, std::vector<std::string> > input = getMultipleNumbersAndOps(results, expressions);
        std::vector<double> numbers = input.first;
        std::vector<std::string> operations = input.second;

        if (numbers.empty()) {
            continue;
        }

        double result = numbers[0];
        std::ostringstream expr;
        expr << numbers[0];

        bool valid = true;
        size_t numIndex = 1;

        for (size_t i = 0; i < operations.size() && valid; ++i) {
            const std::string& op = operations[i];

            if (op == "+") {
                if (numIndex >= numbers.size()) break;
                expr << " + " << numbers[numIndex];
                result += numbers[numIndex++];
            } else if (op == "-") {
                if (numIndex >= numbers.size()) break;
                expr << " - " << numbers[numIndex];
                result -= numbers[numIndex++];
            } else if (op == "*") {
                if (numIndex >= numbers.size()) break;
                expr << " * " << numbers[numIndex];
                result *= numbers[numIndex++];
            } else if (op == "/") {
                if (numIndex >= numbers.size()) break;
                if (numbers[numIndex] == 0) {
                    std::cout << "Error: Division by zero.\n";
                    valid = false;
                    break;
                }
                expr << " / " << numbers[numIndex];
                result /= numbers[numIndex++];
            } else if (op == "^2") {
                expr << "^2";
                result = std::pow(result, 2);
            } else if (op == "^") {
                if (numIndex >= numbers.size()) break;
                expr << " ^ " << numbers[numIndex];
                result = std::pow(result, numbers[numIndex++]);
            } else if (op == "r") {
                if (result < 0) {
                    std::cout << "Error: Square root of negative number.\n";
                    valid = false;
                    break;
                }
                expr << " r";
                result = std::sqrt(result);
            } else if (op == "root") {
                if (numIndex >= numbers.size()) break;
                double n = numbers[numIndex++];
                if (n == 0) {
                    std::cout << "Error: Zeroth root is undefined.\n";
                    valid = false;
                    break;
                }
                expr << " root " << n;
                result = std::pow(result, 1.0 / n);
            } else if (op == "sin") {
                result = std::sin(result * (PI / 180.0));
                expr << " sin ";
            } else if (op == "cos") {
                result = std::cos(result * (PI / 180.0));
                expr << " cos ";
            } else if (op == "tan") {
                result = std::tan(result * (PI / 180.0));
                expr << " tan ";
            } else if (op == "ln") {
                if (result <= 0) {
                    std::cout << "Error: Natural log of non-positive number.\n";
                    valid = false;
                    break;
                }
                result = std::log(result);
                expr << " ln ";
            } else if (op == "log") {
                if (result <= 0) {
                    std::cout << "Error: Log of non-positive number.\n";
                    valid = false;
                    break;
                }
                result = std::log10(result);
                expr << " log ";
            } else {
                std::cout << "Unknown operation: " << op << "\n";
                valid = false;
                break;
            }
        }

        if (valid && !operations.empty()) {
            expr << " = " << result;
            results.push_back(result);
            expressions.push_back(expr.str());
            drawHUD(results, expressions);
            std::cout << "Result: " << result << "\n";
        }

        std::cout << "\nDo you want to continue? (Y/N): ";
        std::cin >> cont;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Exiting...\n";
    return 0;
}
