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

double getNumberInput(const std::vector<double>& results, const std::vector<std::string>& expressions, const std::string& message) {
    std::string input;
    double value;
    while (true) {
        std::cout << message;
        std::cin >> input;
        if (input == "r" || input == "R") {
            showResults(results);
            showExpressions(expressions);
        } else {
            std::stringstream ss(input);
            if (ss >> value) return value;
            std::cout << "Invalid input. Try again.\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

int main() {
    char op;
    double num1, num2, result;
    char cont = 'y';
    bool usedPrev;

    std::vector<double> results;
    std::vector<std::string> expressions;

    std::cout << "CALCULATOR\n";
    std::cout << "\nUse R at any point to see stored results and expressions.\n\n";

    while (cont == 'y' || cont == 'Y') {
        usedPrev = false;

        if (!results.empty()) {
            std::cout << "\nYou can use previous results for inputs.\n";
            int useCount = static_cast<int>(getNumberInput(results, expressions, "How many previous results to use? (0, 1, or 2): "));

            if (useCount == 1) {
                showResults(results);
                int numch = static_cast<int>(getNumberInput(results, expressions, "Use result for num1 or num2? (1/2): "));
                int index = static_cast<int>(getNumberInput(results, expressions, "Enter result number to use: "));

                if (index >= 1 && index <= static_cast<int>(results.size())) {
                    if (numch == 1) {
                        num1 = results[index - 1];
                        num2 = getNumberInput(results, expressions, "Enter number 2: ");
                        usedPrev = true;
                    } else if (numch == 2) {
                        num1 = getNumberInput(results, expressions, "Enter number 1: ");
                        num2 = results[index - 1];
                        usedPrev = true;
                    }
                } else {
                    std::cout << "Invalid result index. Proceeding with manual input.\n";
                }
            } else if (useCount == 2) {
                showResults(results);
                int index1 = static_cast<int>(getNumberInput(results, expressions, "Enter result number for num1: "));
                int index2 = static_cast<int>(getNumberInput(results, expressions, "Enter result number for num2: "));

                if (index1 >= 1 && index1 <= static_cast<int>(results.size()) &&
                    index2 >= 1 && index2 <= static_cast<int>(results.size())) {
                    num1 = results[index1 - 1];
                    num2 = results[index2 - 1];
                    usedPrev = true;
                } else {
                    std::cout << "Invalid result index(es). Proceeding with manual input.\n";
                }
            }
        }

        if (!usedPrev) {
            num1 = getNumberInput(results, expressions, "Enter number 1: ");
            num2 = getNumberInput(results, expressions, "Enter number 2: ");
        }

        op = getCharInput(results, expressions, "Enter operation (+, -, *, /): ");

        bool validOperation = true;
        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    std::cout << "Error: Division by zero is not allowed.\n";
                    validOperation = false;
                } else {
                    result = num1 / num2;
                }
                break;
            default:
                std::cout << "Please enter a valid operation!\n";
                validOperation = false;
                break;
        }

        if (validOperation) {
            results.push_back(result);
            std::ostringstream expr;
            expr << num1 << " " << op << " " << num2 << " = " << result;
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
