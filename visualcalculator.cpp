#include "visualcalculator.h"
#include <QClipboard>
#include <ui_standartvisualcalculator.h>
#include <ui_integralvisualvalculator.h>

//you need to connect a third-party project with the CalculatorUtils namespace(https://github.com/YuarenArt/CalculatorUtils), 
//which defines functions for calculations and transformations that do not depend on the Visualcalculator class

#include "\QT Lib\CalculatorUtils\CalculatorUtils.cpp"
using namespace CalculatorUtils;

VisualCalculator::VisualCalculator(QWidget* parent) : QMainWindow(parent), ui(new Ui::standartVisualCalculator), ui_integral(nullptr)
{
    generateStandartInterface();
}

void VisualCalculator::generateStandartInterface()
{
    if (ui_integral) {
        delete ui_integral;
        ui_integral = nullptr;
    }

    /*ui_integral = new Ui_integral::integralVisualCalculator;
    QWidget* integralWidget = new QWidget(this);
    ui_integral->setupUi(integralWidget);

    setCentralWidget(integralWidget);*/

    ui = new Ui::standartVisualCalculator;
    ui->setupUi(this);

    QMenu* menu = new QMenu(ui->menuChoice);

    // �������� �������� (������� ����)
    QAction* setDefault = new QAction("Default calculator", menu);
    QAction* setIntegral = new QAction("Numerical Integral Calculator", menu);

    // ���������� �������� � ����
    menu->addAction(setDefault);
    menu->addAction(setIntegral);
    // ��������� ���� ��� QToolButton
    ui->menuChoice->setMenu(menu);
    // ��������� ����� ������������ ����
    ui->menuChoice->setPopupMode(QToolButton::InstantPopup);

    connect(setDefault, &QAction::triggered, this, &VisualCalculator::loadStandardInterface);
    connect(setIntegral, &QAction::triggered, this, &VisualCalculator::updateInterfaceIntegral);

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &VisualCalculator::updateDisplayText);
    connect(ui->equalBtn, &QPushButton::clicked, this, &VisualCalculator::calculateResult);
    connect(ui->copyBtn, &QPushButton::clicked, this, &VisualCalculator::copyExpressionToClipboard);
    connect(ui->clearBtn, &QPushButton::clicked, this, &VisualCalculator::clearExpression);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &VisualCalculator::handleEnterPressed);
    connect(ui->historyList, &QListWidget::itemDoubleClicked, this, &VisualCalculator::historyListDoubleClicked);
}

VisualCalculator::~VisualCalculator()
{
    delete ui;
    delete ui_integral;
}

// ������� ��������� ������� ��������� � ������ 
void VisualCalculator::updateHistoryList(const QString& text)
{
    if (ui) {
        ui->historyList->addItem(text);
    }

    else {
        ui_integral->historyList->addItem(text);
    }
}

// ������� ��������� ���������
void VisualCalculator::clearExpression()
{
    if (ui) {
        ui->lineEdit->clear();
        calculateResult();
    }
    else {
        ui_integral->lineEdit->clear();
        calculateResult();
    }
}

//
void VisualCalculator::calculateResult()
{
    if (ui) {
        QString expression = ui->textShow->toPlainText();

        // ��������� ������ ����
        if (expression.isEmpty())
        {
            // ������� ���������
            ui->showResult->setText(" ");
            return;
        }
        // ��������� ���������
        double result = calculateExpression(expression);

        updateHistoryList(expression);

        // ��������� ����� � showResult
        ui->showResult->setText(QString::number(result));
    }
    else {
        QString expression = ui_integral->textShow->toPlainText();

        // ��������� ������ ����
        if (expression.isEmpty())
        {
            // ������� ���������
            ui_integral->showResult->setText(" ");
            return;
        }
        // ��������� ���������
        double result = calculateExpression(expression);

        updateHistoryList(expression);

        // ��������� ����� � showResult
        ui_integral->showResult->setText(QString::number(result));
    }
}

// ������� ��������� ������������� ���������

void VisualCalculator::updateDisplayText(const QString& text)
{
    if (ui)
    {
        QString formattedText = text;

        // ��������� ����������� ����������� ������
        updateText(formattedText);

        // ��������� ����� � textShow
        ui->textShow->setText(formattedText);
    }

    else {
        QString formattedText = text;

        // ��������� ����������� ����������� ������
        updateText(formattedText);

        formattedText.replace(QRegularExpression("(\\d+)(\\w+)"), "\\1 * \\2");
        formattedText.replace(QRegularExpression("(\\w+)([+\\-*/^lgsqrtloglnsincostancottgctg])"), "\\1 \\2");

        // ��������� ����� � textShow
        ui_integral->textShow->setText(formattedText);
    }

}

// ���� �������� ��������� � ����� ������
void VisualCalculator::copyExpressionToClipboard()
{
    if (ui) {
        QString expression = ui->showResult->text();
        QApplication::clipboard()->setText(expression);
    }
    else {
        QString expression = ui_integral->showResult->text();
        QApplication::clipboard()->setText(expression);
    }
}

// ���� ��� ��������� ������� ������� ������� Enter � lineEdit
void VisualCalculator::handleEnterPressed()
{
    calculateResult();
}

// ��� ������ ������ �� ����� ��������� � ����� �������, ��������� ��������� ��������� � lineEdit
void VisualCalculator::historyListDoubleClicked()
{
    if (ui) {
        QListWidgetItem* item = ui->historyList->currentItem();
        ui->lineEdit->setText(item->text());
    }
    else {
        QListWidgetItem* item = ui_integral->historyList->currentItem();
        ui_integral->lineEdit->setText(item->text());
    }
}

// ������ ���������� ���������
void VisualCalculator::calculateResultIntegral()
{
    qlonglong upperLimit = ui_integral->lineEditChoiceUpperLimit->text().toLongLong();
    qlonglong lowerLimit = ui_integral->lineEditChoiceLowerLimit->text().toLongLong();

    QString function = ui_integral->textShow->toPlainText();
    QString variable = ui_integral->lineEditChoiceVariable->text();

    qlonglong result = calculateSimpsonIntegral(function, variable, upperLimit, lowerLimit);
    ui_integral->showResult->setText(QString::number(result));
    updateHistoryList(QString::number(result));
}

// �������������� ��������� ���������� ��� ������� ��������� �������� ����������
void VisualCalculator::updateInterfaceIntegral()
{
    if (ui) {
        delete ui;
        ui = nullptr;
    }

    ui_integral = new Ui_integral::integralVisualCalculator;
    QWidget* integralWidget = new QWidget(this);
    ui_integral->setupUi(integralWidget);

    setCentralWidget(integralWidget);

    QMenu* menu = new QMenu(ui_integral->menuChoice);

    // �������� �������� (������� ����)
    QAction* setDefault = new QAction("Default calculator", menu);
    QAction* setIntegral = new QAction("Numerical Integral Calculator", menu);

    // ���������� �������� � ����
    menu->addAction(setDefault);
    menu->addAction(setIntegral);
    // ��������� ���� ��� QToolButton
    ui_integral->menuChoice->setMenu(menu);
    // ��������� ����� ������������ ����
    ui_integral->menuChoice->setPopupMode(QToolButton::InstantPopup);

    connect(setDefault, &QAction::triggered, this, &VisualCalculator::loadStandardInterface);
    connect(setIntegral, &QAction::triggered, this, &VisualCalculator::updateInterfaceIntegral);

    connect(ui_integral->lineEdit, &QLineEdit::textChanged, this, &VisualCalculator::updateDisplayText);
    connect(ui_integral->equalBtn, &QPushButton::clicked, this, &VisualCalculator::calculateResultIntegral);
    connect(ui_integral->copyBtn, &QPushButton::clicked, this, &VisualCalculator::copyExpressionToClipboard);
    connect(ui_integral->clearBtn, &QPushButton::clicked, this, &VisualCalculator::clearExpression);
    connect(ui_integral->lineEdit, &QLineEdit::returnPressed, this, &VisualCalculator::handleEnterPressed);
    connect(ui_integral->historyList, &QListWidget::itemDoubleClicked, this, &VisualCalculator::historyListDoubleClicked);
}

// ������ ����������� �������� - ������� �����������
void VisualCalculator::loadStandardInterface()
{

    generateStandartInterface();


}

