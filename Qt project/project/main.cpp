#include <QApplication>
#include <QStyle>
#include <QFont>
#include "calculatorwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application style and font
    app.setStyle("Fusion");
    QFont font("Segoe UI", 10);
    app.setFont(font);

    // Create and show calculator
    CalculatorWidget calculator;
    calculator.setWindowTitle("Enhanced Calculator");
    calculator.setFixedSize(480, 720);
    calculator.show();

    return app.exec();
}