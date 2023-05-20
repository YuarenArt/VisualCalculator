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
#include <QStack>
#include <QtWidgets/QApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class VisualCalculator; }
QT_END_NAMESPACE


class VisualCalculator : public QMainWindow
{
    Q_OBJECT

public:
    VisualCalculator(QWidget *parent = nullptr);
    ~VisualCalculator();

    QStringList convertToRPN(const QString& expression);

    double performOperation(double operand1, double operand2, const QString& operation);
    double calculateExpression(const QString& expression);

private:

    Ui::VisualCalculator* ui;
    
public slots:

    void updateTextShow(const QString& text);
    void calculate();
    void copyExpressionToClipboard();


};
