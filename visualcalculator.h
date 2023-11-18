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
namespace Ui { class standartCalculator; }
namespace Ui { class integralCalculator; }
QT_END_NAMESPACE

class VisualCalculator : public QMainWindow
{
    Q_OBJECT

public:
    VisualCalculator(QWidget *parent = nullptr);
    ~VisualCalculator();

    void updateHistoryList(const QString& text);
    void generateStandartInterface();

private:

    Ui::standartCalculator* ui_standart;
    Ui::integralCalculator* ui_integral;

public slots:

    //функции для всех видов калькулятора
    void updateDisplayText(const QString& text);
    void copyExpressionToClipboard();
    void clearExpression();
    void handleEnterPressed();
    void historyListDoubleClicked();

    //функции стандартного калькулятора
    void calculateResult();
    void loadStandardInterface();

    //функции калькулятора интегралов
    void calculateResultIntegral();
    void updateInterfaceIntegral();


};
