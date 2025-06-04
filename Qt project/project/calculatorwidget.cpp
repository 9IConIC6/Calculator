#include "calculatorwidget.h"
#include <QPushButton>
#include <QGridLayout>
#include <cmath>
#include <QMessageBox>

CalculatorWidget::CalculatorWidget(QWidget *parent) : QWidget(parent),
    display(new QLineEdit(this)),
    expressionLabel(new QLabel(this)),
    isNewNumber(true),
    isDecimal(false),
    currentNumber(0.0),
    numbers(),  // Initialize empty QStack<double>
    operators(), // Initialize empty QStack<char>
    currentExpressionPrefix("") // Initialize current expression prefix
{
    // Create buttons
    // Redundant button creations (multiply, divide, add, subtract, equals, clear, backspace, power, root, sin, cos, tan, arcsin, arccos) removed.
    // Their counterparts are created and managed in the createButtons() method.

    buttons.resize(10);  // Initialize button array with size 10

    // Connect buttons
    // Redundant button connections removed.
    // Connections for UI buttons are handled in createButtons() or where those buttons are defined.

    // Create layout
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(4);
    layout->setContentsMargins(8, 8, 8, 8);
    setLayout(layout);

    // Add display and expression label
    layout->addWidget(expressionLabel, 0, 0, 1, 5);
    layout->addWidget(display, 1, 0, 1, 5);

    // Configure display
    display->setAlignment(Qt::AlignRight);
    display->setReadOnly(true);
    display->setMinimumHeight(60);
    display->setStyleSheet("QLineEdit { background-color: rgba(255, 255, 255, 0.1); color: #000000; border: none; font-size: 24px; padding: 10px; }");

    // Configure expression label
    expressionLabel->setAlignment(Qt::AlignRight);
    expressionLabel->setStyleSheet("QLabel { color: #000000; font-size: 16px; padding-right: 10px; }");

    // Initialize state
    numbers.clear();
    operators.clear();

    createButtons(layout);
}

QPushButton* CalculatorWidget::createButton(const QString& text, const QString& style)
{
    QPushButton* button = new QPushButton(text, this);
    button->setMinimumHeight(30);
    button->setMinimumWidth(30);
    button->setStyleSheet(style);
    return button;
}

void CalculatorWidget::createButtons(QGridLayout* layout)
{
    // Create number buttons
    buttons.resize(10);
    for (int i = 0; i < 10; i++) {
        buttons[i] = createButton(QString::number(i), "background-color: rgba(255, 255, 255, 0.3); color: black;");
        connect(buttons[i], &QPushButton::clicked, this, &CalculatorWidget::onNumberClicked);
    }

    // Create operator buttons
    QPushButton *addBtn = createButton("+", "background-color: rgba(255, 165, 0, 0.3);");
    QPushButton *subtractBtn = createButton("-", "background-color: rgba(255, 165, 0, 0.3);");
    QPushButton *multiplyBtn = createButton("*", "background-color: rgba(255, 165, 0, 0.3);");
    QPushButton *divideBtn = createButton("/", "background-color: rgba(255, 165, 0, 0.3);");
    QPushButton *powerOpBtn = createButton("^", "background-color: rgba(255, 165, 0, 0.3);");
    QPushButton *rootOpBtn = createButton("√", "background-color: rgba(255, 165, 0, 0.3);");
    
    connect(addBtn, &QPushButton::clicked, this, &CalculatorWidget::onOperatorClicked);
    connect(subtractBtn, &QPushButton::clicked, this, &CalculatorWidget::onOperatorClicked);
    connect(multiplyBtn, &QPushButton::clicked, this, &CalculatorWidget::onOperatorClicked);
    connect(divideBtn, &QPushButton::clicked, this, &CalculatorWidget::onOperatorClicked);
    connect(powerOpBtn, &QPushButton::clicked, this, &CalculatorWidget::onOperatorClicked);
    connect(rootOpBtn, &QPushButton::clicked, this, &CalculatorWidget::onOperatorClicked);

    // Create scientific function buttons
    QPushButton *sinBtn = createButton("sin", "background-color: rgba(0, 0, 255, 0.3);");
    QPushButton *cosBtn = createButton("cos", "background-color: rgba(0, 0, 255, 0.3);");
    QPushButton *tanBtn = createButton("tan", "background-color: rgba(0, 0, 255, 0.3);");
    QPushButton *arcsinBtn = createButton("sin⁻¹", "background-color: rgba(0, 0, 255, 0.3);");
    QPushButton *arccosBtn = createButton("cos⁻¹", "background-color: rgba(0, 0, 255, 0.3);");

    connect(sinBtn, &QPushButton::clicked, this, &CalculatorWidget::onSinClicked);
    connect(cosBtn, &QPushButton::clicked, this, &CalculatorWidget::onCosClicked);
    connect(tanBtn, &QPushButton::clicked, this, &CalculatorWidget::onTanClicked);
    connect(arcsinBtn, &QPushButton::clicked, this, &CalculatorWidget::onArcSinClicked);
    connect(arccosBtn, &QPushButton::clicked, this, &CalculatorWidget::onArcCosClicked);

    // Create equals button
    QPushButton *equalsBtn = createButton("=", "background-color: rgba(0, 255, 0, 0.3);");
    connect(equalsBtn, &QPushButton::clicked, this, &CalculatorWidget::onEqualsClicked);

    // Create clear button
    QPushButton *clearBtn = createButton("C", "background-color: rgba(255, 0, 0, 0.3);");
    connect(clearBtn, &QPushButton::clicked, this, &CalculatorWidget::onClearClicked);

    // Create backspace button
    QPushButton *backspaceBtn = createButton("←", "background-color: rgba(255, 0, 0, 0.3);");
    connect(backspaceBtn, &QPushButton::clicked, this, &CalculatorWidget::onBackspaceClicked);

    // Create decimal point button
    QPushButton *decimalBtn = createButton(".", "background-color: rgba(255, 255, 255, 0.3); color: black;");
    connect(decimalBtn, &QPushButton::clicked, this, &CalculatorWidget::onNumberClicked);

    // Add buttons to layout
    // Row 1: Scientific/Unary functions (newly added squareBtn here)
    // Row 2: Numbers 7-9, multiply, power
    layout->addWidget(buttons[7], 2, 0);
    layout->addWidget(buttons[8], 2, 1);
    layout->addWidget(buttons[9], 2, 2);
    layout->addWidget(multiplyBtn, 2, 3);
    layout->addWidget(powerOpBtn, 2, 4);
    // Row 3: Numbers 4-6, subtract, root
    layout->addWidget(buttons[4], 3, 0);
    layout->addWidget(buttons[5], 3, 1);
    layout->addWidget(buttons[6], 3, 2);
    layout->addWidget(subtractBtn, 3, 3);
    layout->addWidget(rootOpBtn, 3, 4);
    // Row 4: Numbers 1-3, add, cos
    layout->addWidget(buttons[1], 4, 0);
    layout->addWidget(buttons[2], 4, 1);
    layout->addWidget(buttons[3], 4, 2);
    layout->addWidget(addBtn, 4, 3);
    layout->addWidget(cosBtn, 4, 4);
    // Row 5: Number 0, decimal, equals, divide, sin
    layout->addWidget(buttons[0], 5, 0);
    layout->addWidget(decimalBtn, 5, 1);
    layout->addWidget(equalsBtn, 5, 2);
    layout->addWidget(divideBtn, 5, 3);
    layout->addWidget(sinBtn, 5, 4);
    // Row 6: Clear, backspace, arccos, arcsin, tan
    layout->addWidget(clearBtn, 6, 0);
    layout->addWidget(backspaceBtn, 6, 1);
    layout->addWidget(arccosBtn, 6, 2);
    layout->addWidget(arcsinBtn, 6, 3);
    layout->addWidget(tanBtn, 6, 4);

    // Create View All button
    QPushButton *viewAllBtn = createButton("View All", "background-color: rgba(0, 255, 0, 0.3);");
    connect(viewAllBtn, &QPushButton::clicked, this, &CalculatorWidget::onViewAllClicked);
    layout->addWidget(viewAllBtn, 7, 0, 1, 2);

    // Create Clear History button
    QPushButton *clearHistoryBtn = createButton("Clear History", "background-color: rgba(255, 0, 0, 0.3);");
    connect(clearHistoryBtn, &QPushButton::clicked, this, &CalculatorWidget::onClearHistoryClicked);
    layout->addWidget(clearHistoryBtn, 7, 2, 1, 3);
}

void CalculatorWidget::onNumberClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString buttonText = button->text();
    QString currentDisplayText = display->text(); // Renamed for clarity

    if (isNewNumber) {
        if (buttonText == ".") {
            display->setText("0.");
            isDecimal = true; // Set isDecimal when starting with a decimal point
        } else {
            display->setText(buttonText);
            isDecimal = false; // Reset isDecimal if starting with a non-decimal digit
        }
        isNewNumber = false;
    } else {
        if (buttonText == ".") {
            if (!isDecimal) { // Allow decimal point only if not already present in current number
                display->setText(currentDisplayText + buttonText);
                isDecimal = true;
            }
        } else {
            // Prevent multiple leading zeros if not followed by a decimal
            if (currentDisplayText == "0" && buttonText != ".") {
                 display->setText(buttonText);
            } else {
                 display->setText(currentDisplayText + buttonText);
            }
        }
    }
    // currentNumber is not updated here directly.
    // updateDisplay() is not called here as display->setText() is used directly.
}

void CalculatorWidget::onSinClicked()
{
    if (!isNewNumber) {
        parseDisplay(); // Update currentNumber from display text
    }
    double inputValue = currentNumber;
    currentExpressionPrefix = QString("sin(%1)").arg(inputValue, 0, 'g', 10);
    currentNumber = sin(inputValue * M_PI / 180.0); // Assuming degrees
    expressions.push_back(QString("sin(%1) = %2").arg(inputValue, 0, 'g', 10).arg(currentNumber, 0, 'g', 10));
    results.push_back(currentNumber);
    isNewNumber = true;
    isDecimal = (QString::number(currentNumber).contains('.'));
    updateDisplay();
}

void CalculatorWidget::onCosClicked()
{
    if (!isNewNumber) {
        parseDisplay();
    }
    double inputValue = currentNumber;
    currentExpressionPrefix = QString("cos(%1)").arg(inputValue, 0, 'g', 10);
    currentNumber = cos(inputValue * M_PI / 180.0);
    expressions.push_back(QString("cos(%1) = %2").arg(inputValue, 0, 'g', 10).arg(currentNumber, 0, 'g', 10));
    results.push_back(currentNumber);
    isNewNumber = true;
    isDecimal = (QString::number(currentNumber).contains('.'));
    updateDisplay();
}

void CalculatorWidget::onTanClicked()
{
    if (!isNewNumber) {
        parseDisplay();
    }
    double inputValue = currentNumber;
    currentExpressionPrefix = QString("tan(%1)").arg(inputValue, 0, 'g', 10);
    // Avoid tan(90), tan(270), etc.
    double angleRad = inputValue * M_PI / 180.0;
    if (std::abs(std::cos(angleRad)) < 1e-12) { // Check if cos(angle) is close to zero
        QMessageBox::warning(this, "Error", "Tangent is undefined for this angle.");
        // Optionally reset currentNumber or leave as is
        display->setText("Error");
        currentNumber = 0; // Or keep inputValue
        isNewNumber = true;
        isDecimal = false;
        return;
    }
    currentNumber = tan(angleRad);
    expressions.push_back(QString("tan(%1) = %2").arg(inputValue, 0, 'g', 10).arg(currentNumber, 0, 'g', 10));
    results.push_back(currentNumber);
    isNewNumber = true;
    isDecimal = (QString::number(currentNumber).contains('.'));
    updateDisplay();
}

void CalculatorWidget::onArcSinClicked()
{
    if (!isNewNumber) {
        parseDisplay();
    }
    double inputValue = currentNumber;
    currentExpressionPrefix = QString("asin(%1)").arg(inputValue, 0, 'g', 10);
    if (inputValue < -1.0 || inputValue > 1.0) {
        QMessageBox::warning(this, "Error", "Arcsin input must be between -1 and 1.");
        display->setText("Error");
        currentNumber = 0; // Or keep inputValue
        isNewNumber = true;
        isDecimal = false;
        return;
    }
    currentNumber = asin(inputValue) * 180.0 / M_PI; // Result in degrees
    expressions.push_back(QString("asin(%1) = %2").arg(inputValue, 0, 'g', 10).arg(currentNumber, 0, 'g', 10));
    results.push_back(currentNumber);
    isNewNumber = true;
    isDecimal = (QString::number(currentNumber).contains('.'));
    updateDisplay();
}

void CalculatorWidget::onArcCosClicked()
{
    if (!isNewNumber) {
        parseDisplay();
    }
    double inputValue = currentNumber;
    currentExpressionPrefix = QString("acos(%1)").arg(inputValue, 0, 'g', 10);
    if (inputValue < -1.0 || inputValue > 1.0) {
        QMessageBox::warning(this, "Error", "Arccos input must be between -1 and 1.");
        display->setText("Error");
        currentNumber = 0; // Or keep inputValue
        isNewNumber = true;
        isDecimal = false;
        return;
    }
    currentNumber = acos(inputValue) * 180.0 / M_PI; // Result in degrees
    expressions.push_back(QString("acos(%1) = %2").arg(inputValue, 0, 'g', 10).arg(currentNumber, 0, 'g', 10));
    results.push_back(currentNumber);
    isNewNumber = true;
    isDecimal = (QString::number(currentNumber).contains('.'));
    updateDisplay();
}

void CalculatorWidget::onOperatorClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString opText = button->text(); // e.g., "+", "-", "*", "/", "^", "√"

    if (!isNewNumber) {
        // User was typing a number. Parse it, push to stack.
        parseDisplay(); // currentNumber is updated
        numbers.push(currentNumber);
        // Now update prefix: append the number just pushed and the new operator.
        currentExpressionPrefix += QString::number(currentNumber, 'g', 10) + " " + opText + " ";
    } else {
        // isNewNumber is true. Operator follows a result or another operator.
        // currentNumber holds the first operand.
        // If the numbers stack is empty OR if the top of the stack isn't this currentNumber 
        // (e.g. after a scientific function that only updated currentNumber and didn't push, or after C, or after = if stacks were cleared),
        // push currentNumber.
        if (numbers.isEmpty() || numbers.top() != currentNumber) { 
            numbers.push(currentNumber);
        }
        // Update prefix: it starts fresh with currentNumber and the new operator.
        currentExpressionPrefix = QString::number(currentNumber, 'g', 10) + " " + opText + " ";
    }

    operators.push(opText.at(0).toLatin1());

    isNewNumber = true;  // The next digit typed will start a new number.
    isDecimal = false;   // Reset decimal status for the new number input.

    // updateDisplay() will show currentNumber in the main display (which is the number just pushed or a previous result)
    // and attempt to update the expressionLabel based on stack states.
    updateDisplay();
}

void CalculatorWidget::onEqualsClicked()
{
    if (!isNewNumber) { // A number is in the display, not yet processed
        parseDisplay(); // currentNumber = display->text().toDouble()
        numbers.push(currentNumber);
    }

    // If there are no operators, or not enough numbers for an operation, do nothing further.
    // The display will show the currentNumber (which might be the number just parsed).
    if (operators.isEmpty() || numbers.size() < 2) { // Assuming binary operators for this check
        if (numbers.size() == 1 && operators.isEmpty()) { // e.g. user typed "5" then "="
            currentNumber = numbers.top(); // Keep the number as current
        }
        // else if numbers is empty and operators is empty, currentNumber is likely 0 or from C/CE
        updateDisplay();
        isNewNumber = true; // Ready for new input
        isDecimal = (QString::number(currentNumber).contains('.'));
        return;
    }

    // Build expression string for history BEFORE calculation modifies stacks
    QString expressionForHistory;
    {
        QStack<double> tempNum = numbers;    // Copy for building expression string
        QStack<char> tempOp = operators; // Copy for building expression string
        QStringList parts;
        while(!tempNum.isEmpty()){
            parts.prepend(QString::number(tempNum.pop(), 'g', 10));
            if(!tempOp.isEmpty()){
                if (!parts.isEmpty()) parts.prepend(" ");
                parts.prepend(QString(QChar(tempOp.pop())));
                parts.prepend(" ");
            }
        }
        expressionForHistory = parts.join("");
    }

    // Perform calculations
    while (!operators.isEmpty() && numbers.size() >= 2) {
        char opChar = operators.pop(); // Changed to char directly
        double b = numbers.pop();
        double a = numbers.pop();

        if (opChar == '√') { // Assuming 'a √ b' means b-th root of a => pow(a, 1.0/b)
            if (a < 0 && (static_cast<int>(b) % 2 == 0 && b != 0) ) { // Even root of negative number
                QMessageBox::warning(this, "Error", "Cannot take even root of negative number");
                numbers.push(a); numbers.push(b); operators.push(opChar); // Rollback
                return;
            }
            if (b == 0) { // Root index is 0
                 QMessageBox::warning(this, "Error", "Root index cannot be zero.");
                 numbers.push(a); numbers.push(b); operators.push(opChar); // Rollback
                 return;
            }
            numbers.push(pow(a, 1.0/b));
        } else if (opChar == '+') {
            numbers.push(a + b);
        } else if (opChar == '-') {
            numbers.push(a - b);
        } else if (opChar == '*') {
            numbers.push(a * b);
        } else if (opChar == '/') {
            if (b == 0) {
                QMessageBox::warning(this, "Error", "Division by zero");
                numbers.push(a); numbers.push(b); operators.push(opChar); // Rollback
                return;
            }
            numbers.push(a / b);
        } else if (opChar == '^') {
            numbers.push(pow(a, b));
        } else {
            QMessageBox::warning(this, "Error", "Unknown operator: " + QString(opChar));
            numbers.push(a); numbers.push(b); operators.push(opChar); // Rollback
            return;
        }
    }

    // After calculation loop, the result should be the single item left in numbers stack.
    if (!numbers.isEmpty()) {
        currentNumber = numbers.pop(); // The final result

        // Store result and expression for history
        results.push_back(currentNumber);
        if (!expressionForHistory.isEmpty()) {
            expressions.push_back(expressionForHistory + " = " + QString::number(currentNumber, 'g', 10));
        }
    } else {
        // This case should ideally not be reached if logic is correct and an error didn't occur
        // or if the expression was incomplete. currentNumber might hold an intermediate value or be stale.
    }

    currentExpressionPrefix.clear();
    isNewNumber = true;  // Next input will start a new number.
    isDecimal = (QString::number(currentNumber).contains('.')); // Set based on the result.
    updateDisplay(); // Show currentNumber (the result) and update expressionLabel.
}


void CalculatorWidget::onClearClicked() // Handles 'C' button
{
    currentNumber = 0.0;
    isNewNumber = true;
    isDecimal = false;
    // decimalPlaces was removed
    numbers.clear();
    operators.clear();
    display->setText("0"); // Directly set display to 0
    expressionLabel->clear();
    currentExpressionPrefix.clear(); // Clear expression label
    // updateDisplay(); // Calling updateDisplay() will also work by setting display from currentNumber and clearing expr label based on stack states.
}

void CalculatorWidget::onClearHistoryClicked()
{
    results.clear();
    expressions.clear();
    QMessageBox::information(this, "History Cleared", "Calculator history has been cleared.");
}

void CalculatorWidget::onBackspaceClicked()
{
    if (isNewNumber) { // If a new number is expected, backspace does nothing to the display
        return;
    }

    QString currentText = display->text();
    if (currentText == "0" || currentText.isEmpty()) {
        return; // Nothing to remove from "0" or empty
    }

    // Check if the character being removed is a decimal point
    if (currentText.endsWith('.')) {
        isDecimal = false;
    }

    currentText.chop(1); // Remove the last character

    if (currentText.isEmpty() || currentText == "-") { // If string becomes empty or just a minus sign
        display->setText("0");
        isNewNumber = true; // Expect a new number now
        isDecimal = false;
    } else {
        display->setText(currentText);
        // isNewNumber remains false, user is still editing
    }
    updateDisplay(); // Update expression label
}

void CalculatorWidget::onViewAllClicked()
{
    if (expressions.isEmpty()) {
        QMessageBox::information(this, "History", "No calculations in history.");
        return;
    }

    QString historyText;
    for (const QString &expr : expressions) {
        historyText += expr + "\n";
    }

    QMessageBox::information(this, "Calculation History", historyText);
}

void CalculatorWidget::parseDisplay()
{
    QString text = display->text();
    bool ok;
    double number = text.toDouble(&ok);
    if (ok) {
        currentNumber = number;
    } else {
        // If conversion fails, display an error and reset currentNumber.
        // This situation should be rare if onNumberClicked correctly validates input.
        display->setText("Error");
        currentNumber = 0.0;
        isNewNumber = true; // Expect a new number after error
        isDecimal = false;
    }
}

void CalculatorWidget::updateDisplay()
{
    if (isNewNumber) {
        display->setText(QString::number(currentNumber, 'g', 10));
    }

    if (isNewNumber) {
        if (!currentExpressionPrefix.isEmpty()) {
            expressionLabel->setText(currentExpressionPrefix);
        } else if (!expressions.isEmpty()) {
            expressionLabel->setText(expressions.last());
        } else {
            expressionLabel->clear();
        }
    } else {
        expressionLabel->setText(currentExpressionPrefix + display->text());
    }
}
