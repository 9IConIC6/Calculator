#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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


double getNumberInput(const std::vector<double>& results, const std::string& message) {
    std::string input;
    double value;
    while (true) {
        std::cout << message;
        std::cin >> input;
        if (input == "r" || input == "R") {
            showResults(results);  
        } else {
            std::stringstream ss(input);
            if (ss >> value) return value;
            std::cout << "Invalid input. Try again.\n";
        }
    }
}

char getCharInput(const std::vector<double>& results, const std::string& message) {
    std::string input;
    while (true) {
        std::cout << message;
        std::cin >> input;
        if (input == "r" || input == "R") {
            showResults(results); 
        } else if (input.length() == 1) {
            return input[0];
        } else {
            std::cout << "Invalid input. Try again.\n";
        }
    }
}
int main() {
	
	char op;
	double num1,num2,result;
	char cont= 'y';
	char prev;
	int numch;
	bool usedPrev;
	
	std::vector<double> results; 
	std::vector<std::string> expressions;
	
	std::cout << "CALCULATOR\n";
	std::cout << "\nUse R at any point to see stored results\n\n";
	
	while (cont == 'y'|| cont == 'Y') {
        usedPrev = false;

        if (!results.empty()) {
            prev = getCharInput(results, "\nDo you want to use a previous result for one number? (Y/N): ");
            if (prev == 'y' || prev == 'Y') {
                showResults(results);
                numch = static_cast<int>(getNumberInput(results, "Use result for num1 or num2? (1/2): "));
                int index = static_cast<int>(getNumberInput(results, "Enter result number to use: "));

                if (index >= 1 && index <= static_cast<int>(results.size())) {
                    if (numch == 1) {
                        num1 = results[index - 1];
                        num2 = getNumberInput(results, "Enter number 2: ");
                        usedPrev = true;
                    } else if (numch == 2) {
                        num1 = getNumberInput(results, "Enter number 1: ");
                        num2 = results[index - 1];
                        usedPrev = true;
                    } else {
                        std::cout << "Invalid input. Proceeding with manual input.\n";
                    }
                } else {
                    std::cout << "Invalid result index. Proceeding with manual input.\n";
                }
            }
        }

        if (!usedPrev) {
            num1 = getNumberInput(results, "Enter number 1: ");
            num2 = getNumberInput(results, "Enter number 2: ");
        }

        op = getCharInput(results, "Enter operation (+, -, *, /): ");

        switch (op) {
            case '+':
                result = num1 + num2;
                results.push_back(result);
                std::cout << "result: " << result << "\n";
                break;
            case '-':
                result = num1 - num2;
                results.push_back(result);
                std::cout << "result: " << result << "\n";
                break;
            case '*':
                result = num1 * num2;
                results.push_back(result);
                std::cout << "result: " << result << "\n";
                break;
            case '/':
                if (num2 == 0) {
                    std::cout << "Error: Division by zero is not allowed.\n";
                } else {
                    result = num1 / num2;
                    results.push_back(result);
                    std::cout << "result: " << result << "\n";
                }
                break;
            default:
                std::cout << "Please enter a valid operation!\n";
                break;
        }
        std::ostringstream expr;
        expr << num1 << " " << op << " " << num2 << " = " << result;
        expressions.push_back(expr.str());

        std::cout << "Result: " << result << "\n";


        cont = getCharInput(results, "\nDo you want to continue? (Y/N): ");
    }
}
    std::cout << "\nStored results:\n";
    showExpressions(expressions);

    return 0;
    
}
