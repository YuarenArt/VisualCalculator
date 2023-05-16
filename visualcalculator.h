#pragma once

#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QRegularExpression>
#include <QMessageBox>

class VisualCalculator : public QMainWindow
{
    Q_OBJECT

public:
    VisualCalculator(QWidget *parent = nullptr);

    QStringList convertToRPN(const QString& expression);

    bool isValidInput(const QString& input);
    bool isOperator(const QString& token);
    double performOperation(double operand1, double operand2, const QString& operation);
    double calculateExpression(const QString& expression);

private:
    QTextEdit* textShow;
    QLineEdit* lineEdit;
    QPushButton* equalBtn;
    QLineEdit* showResult;
    
public slots:

    void updateTextShow(const QString& text);
    void calculate();

};
