#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <QWidget>
#include <QStack>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class CalculatorWidget : public QWidget {
    Q_OBJECT

public:
    explicit CalculatorWidget(QWidget *parent = nullptr);

private slots:
    void onNumberClicked();
    void onOperatorClicked();
    void onEqualsClicked();
    void onClearClicked();
    void onBackspaceClicked();
    void onViewAllClicked();
    void onClearHistoryClicked();
    void onSinClicked();
    void onCosClicked();
    void onTanClicked();
    void onArcSinClicked();
    void onArcCosClicked();

private:
    QLineEdit *display;
    QLabel *expressionLabel;
    bool isNewNumber;
    bool isDecimal;
    double currentNumber;
    // int decimalPlaces; // No longer needed with string-based input
    QVector<QPushButton*> buttons;
    QStack<double> numbers;
    QStack<char> operators;
    QVector<double> results;
    QVector<QString> expressions;
    QString currentExpressionPrefix; // Stores the current expression being built

    QPushButton* createButton(const QString& text, const QString& style = "");
    void createButtons(QGridLayout* layout);
    void parseDisplay(); // Helper to convert display text to currentNumber

    void performCalculation();
    void updateDisplay();
};

#endif // CALCULATORWIDGET_H
