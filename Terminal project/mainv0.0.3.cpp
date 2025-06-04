#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>


void showResults(const std::vector<double>& results) {
    if (results.empty()) {
        std::cout << "No stored results.\n";
    } else {
        std::cout << "\nStored results:\n";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << i + 1 << ": " << results[i] << '\n';
        }
    }
}

void showExpressions(const std::vector<std::string>& expressions) {
    if (expressions.empty()) {
        std::cout << "No stored expressions.\n";
    } else {
        std::cout << "\nStored calculations:\n";
        for (size_t i = 0; i < expressions.size(); ++i) {
            std::cout << i + 1 << ": " << expressions[i] << '\n';
        }
    }
}

char getCharInput(const std::vector<double>& results, const std::vector<std::string>& expressions, const std::string& message) {
    std::string input;
    while (true) {
        std::cout << message;
        std::cin >> input;
        if (input == "r" || input == "R") {
            showResults(results);
            showExpressions(expressions);
        } else if (input.length() == 1) {
            return input[0];
        } else {
            std::cout << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::vector<double> getMultipleNumbers(const std::vector<double>& results, const std::vector<std::string>& expressions) {
    std::vector<double> numbers;
    std::string input;

    std::cout << "Enter numbers one by one (type 'done' to finish, or 'R' to show results):\n";

    while (true) {
        std::cout << "Enter number " << numbers.size() + 1 << ": ";
        std::cin >> input;

        if (input == "done") {
            if (numbers.size() >= 2) break;
            std::cout << "Please enter at least two numbers.\n";
        } else if (input == "R" || input == "r") {
            showResults(results);
            showExpressions(expressions);
        } else if (input[0] == '#' && input.length() > 1) {
            int index;
            std::istringstream(input.substr(1)) >> index;
            if (index >= 1 && index <= static_cast<int>(results.size())) {
                numbers.push_back(results[index - 1]);
            } else {
                std::cout << "Invalid result index.\n";
            }
        } else {
            std::stringstream ss(input);
            double value;
            if (ss >> value) {
                numbers.push_back(value);
            } else {
                std::cout << "Invalid number. Try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    return numbers;
}

int main() {
    std::vector<double> results;
    std::vector<std::string> expressions;
    char cont = 'y';

    std::cout << "MULTI-NUMBER CALCULATOR\n";
    std::cout << "You can type 'R' at any prompt to review stored results/expressions.\n";
    std::cout << "You can also type '#n' to use result number n (e.g., #2) when entering numbers.\n\n";

    while (cont == 'y' || cont == 'Y') {
        std::vector<double> numbers = getMultipleNumbers(results, expressions);
        char op = getCharInput(results, expressions, "Enter operation to apply (+, -, *, /): ");

        if (numbers.empty()) continue;

        double result = numbers[0];
        std::ostringstream expr;
        expr << numbers[0];

        bool valid = true;

        for (size_t i = 1; i < numbers.size(); ++i) {
            expr << " " << op << " " << numbers[i];

            switch (op) {
                case '+': result += numbers[i]; break;
                case '-': result -= numbers[i]; break;
                case '*': result *= numbers[i]; break;
                case '/':
                    if (numbers[i] == 0) {
                        std::cout << "Error: Division by zero.\n";
                        valid = false;
                    } else {
                        result /= numbers[i];
                    }
                    break;
                default:
                    std::cout << "Invalid operation.\n";
                    valid = false;
                    break;
            }

            if (!valid) break;
        }

        if (valid) {
            expr << " = " << result;
            results.push_back(result);
            expressions.push_back(expr.str());
            std::cout << "Result: " << result << "\n";
        }

        cont = getCharInput(results, expressions, "\nDo you want to continue? (Y/N): ");
    }

    std::cout << "\nFinal Stored Results:\n";
    showResults(results);
    std::cout << "\nFinal Stored Expressions:\n";
    showExpressions(expressions);

    return 0;
}
