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

void drawButton(const std::string& text, bool selected = false) {
    if (selected) std::cout << "[" << text << "]";
    else std::cout << " " << text << " ";
    std::cout << "  ";
}

void drawHUD(const std::vector<double>& results, const std::vector<std::string>& expressions) {
    const int width = 70;
    const int height = 22;
    char screen[height][width + 1];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || i == height - 1) screen[i][j] = '=';
            else if (j == 0 || j == width - 1) screen[i][j] = '|';
            else screen[i][j] = ' ';
        }
        screen[i][width] = '\0';
    }

    std::string title = " ENHANCED CALCULATOR ";
    int titlePos = (width - title.length()) / 2;
    for (size_t i = 0; i < title.size() && titlePos + i < width - 1; ++i) {
        screen[1][titlePos + i] = title[i];
    }

    if (!results.empty()) {
        std::ostringstream oss;
        oss << "Last result: " << results.back();
        std::string resStr = oss.str();
        for (size_t i = 0; i < resStr.size() && i + 4 < width - 1; ++i) {
            screen[3][4 + i] = resStr[i];
        }
    }

    if (!expressions.empty()) {
        std::ostringstream oss;
        oss << "Expression: " << expressions.back();
        std::string exprStr = oss.str();
        for (size_t i = 0; i < exprStr.size() && i + 4 < width - 1; ++i) {
            screen[5][4 + i] = exprStr[i];
        }
    }

    std::cout << "\n";
    for (int i = 0; i < height; ++i) {
        std::cout << screen[i] << "\n";
    }

    std::cout << "\n  ";
    drawButton("1"); drawButton("2"); drawButton("3"); drawButton("+"); drawButton("^2");
    std::cout << "\n  ";
    drawButton("4"); drawButton("5"); drawButton("6"); drawButton("-"); drawButton("r");
    std::cout << "\n  ";
    drawButton("7"); drawButton("8"); drawButton("9"); drawButton("*"); drawButton("root");
    std::cout << "\n  ";
    drawButton("e"); drawButton("0"); drawButton("p"); drawButton("/"); drawButton("^");
    std::cout << "\n  ";
    drawButton("sin"); drawButton("sin^-1"); drawButton("cos"); drawButton("cos^-1");
    std::cout << "\n  ";
    drawButton("tan"); drawButton("ln"); drawButton("log"); drawButton("#n");
    std::cout << "\n  ";
    drawButton("=");
    std::cout << "\n  ";
    drawButton("View All (v)"); drawButton("Clear (c)"); drawButton("Done (d)");
    std::cout << "\n\n";
}

void clearResults(std::vector<double>& results, std::vector<std::string>& expressions) {
    results.clear();
    expressions.clear();
    std::cout << "All results and expressions have been cleared.\n";
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
        std::cout << "Enter operation (+, -, *, /, ^2, ^, r, root, sin, sin^-1, cos, cos^-1, tan, ln, log), 'v' to view all, 'c' to clear, or 'done' to finish: ";
        std::cin >> op;
        for (std::size_t i = 0; i < op.size(); ++i)
            op[i] = std::tolower(op[i]);

        if (op == "v") {
            viewAll(results, expressions);
            continue;
        } else if (op == "c" || op == "clear") {
            clearResults(const_cast<std::vector<double>&>(results), const_cast<std::vector<std::string>&>(expressions));
            return "clear";
        } else if (op == "done" || op == "d") {
            return "done";
        } else if (op == "+" || op == "-" || op == "*" || op == "/" ||
                   op == "^" || op == "^2" || op == "r" || op == "root" ||
                   op == "sin" || op == "sin^-1" || op == "cos" || op == "cos^-1" ||
                   op == "tan" || op == "ln" || op == "log") {
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
        std::cout << "Number " << numbers.size() + 1 << " (or 'done'/'d' to finish, 'v' to view all): ";
        std::cin >> input;

        if (input == "done" || input == "d") {
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
            while (true) {
                std::string op = getOperationInput(results, expressions);
                if (op == "v") {
                    viewAll(results, expressions);
                    continue;
                } else if (op == "clear") {
                    clearResults(const_cast<std::vector<double>&>(results), const_cast<std::vector<std::string>&>(expressions));
                    return std::make_pair(std::vector<double>(), std::vector<std::string>());
                } else if (op.empty() || op == "done") {
                    break;
                }
                operations.push_back(op);
                
                if (op == "^2" || op == "r" || op == "sin" || op == "sin^-1" || 
                    op == "cos" || op == "cos^-1" || op == "tan" || op == "ln" || op == "log") {
                    return std::make_pair(numbers, operations);
                }
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

    std::cout << "Launching Enhanced Calculator with Button Interface...\n";
    std::cout << "Use the buttons as a reference for available operations.\n";
    drawHUD(results, expressions);

    while (std::tolower(cont) == 'y') {
        std::pair<std::vector<double>, std::vector<std::string> > input = getMultipleNumbersAndOps(results, expressions);
        std::vector<double> numbers = input.first;
        std::vector<std::string> operations = input.second;

        if (numbers.empty()) {
            if (!operations.empty() && operations[0] == "clear") {
                drawHUD(results, expressions);
            }
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
            } else if (op == "sin^-1" || op == "asin") {
                if (result < -1.0 || result > 1.0) {
                    std::cout << "Error: asin(x) is only defined for -1 = x = 1\n";
                    valid = false;
                    break;
                }
                result = std::asin(result) * (180.0 / PI);
                expr << " sin^-1 ";
            } else if (op == "cos^-1" || op == "acos") {
                if (result < -1.0 || result > 1.0) {
                    std::cout << "Error: acos(x) is only defined for -1 = x = 1\n";
                    valid = false;
                    break;
                }
                result = std::acos(result) * (180.0 / PI);
                expr << " cos^-1 ";
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

