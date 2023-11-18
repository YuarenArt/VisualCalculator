#include "visualcalculator.h"
#include <QClipboard>
#include <ui_standartvisualcalculator.h>
#include <ui_integralvisualvalculator.h>

//you need to connect a third-party project with the CalculatorUtils namespace(https://github.com/YuarenArt/CalculatorUtils), 
//which defines functions for calculations and transformations that do not depend on the Visualcalculator class

#include "CalculatorUtils\CalculatorUtils.cpp"
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

    ui = new Ui::standartVisualCalculator;
    ui->setupUi(this);

    // Create and set up a drop-down menu to switch between the standard calculator and the integral calculator
    QMenu* menu = new QMenu(ui->menuChoice);
    QAction* setDefault = new QAction(tr("Default calculator"), menu);
    QAction* setIntegral = new QAction(tr("Numerical Integral Calculator"), menu);
    menu->addAction(setDefault);
    menu->addAction(setIntegral);
    ui->menuChoice->setMenu(menu);
    ui->menuChoice->setPopupMode(QToolButton::InstantPopup);

    // Connect the actions to their corresponding slots
    connect(setDefault, &QAction::triggered, this, &VisualCalculator::loadStandardInterface);
    connect(setIntegral, &QAction::triggered, this, &VisualCalculator::updateInterfaceIntegral);

    // Connect various UI elements to their respective slots for user interaction
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

// Function to add expressions to the history list
void VisualCalculator::updateHistoryList(const QString& text)
{
    if (ui) {
        ui->historyList->addItem(text);
    }

    else {
        ui_integral->historyList->addItem(text);
    }
}

// Function to clear the input expression
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

// Function to calculate the result of the expression and update the display
void VisualCalculator::calculateResult()
{
    if (ui) {
        QString expression = ui->textShow->toPlainText();

        if (expression.isEmpty())
        {
            ui->showResult->setText(" "); // Clear the result display if the expression is empty
            return;
        }

        double result = calculateExpressionWithRPN(expression); // Perform the actual calculation

        updateHistoryList(expression);

        ui->showResult->setText(QString::number(result)); // Display the result
    }
    else {
        // вычисляем выражение
        calculateResultIntegral();
    }
}

// Function to update the displayed expression in the text box
void VisualCalculator::updateDisplayText(const QString& text)
{
    if (ui)
    {
        QString formattedText = text;

        // Добавляем недостающие закрывающие скобки
        updateText(formattedText);

        // Обновляем текст в textShow
        ui->textShow->setText(formattedText);
    }

    else {
        QString formattedText = text;

       
        updateText(formattedText);

        QString variable = ui_integral->lineEditChoiceVariable->text();

        formattedText.replace(QRegularExpression("(\\d+)([a-zA-Z])"), "\\1*\\2");

        // Обновляем текст в textShow
        ui_integral->textShow->setText(formattedText);
    }

}

// Function to copy the result to the clipboard
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

// Slot for handling the Enter key press event in the lineEdit
void VisualCalculator::handleEnterPressed()
{
    calculateResult();
}

// Slot for handling the double-click event on the history list
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

// Function to calculate the result of the integral expression
void VisualCalculator::calculateResultIntegral()
{
    double upperLimit = ui_integral->lineEditChoiceUpperLimit->text().toDouble();
    double lowerLimit = ui_integral->lineEditChoiceLowerLimit->text().toDouble();

    QString function = ui_integral->textShow->toPlainText();
    QString variable = ui_integral->lineEditChoiceVariable->text();

    double result = calculateIntegralSimpsonsMethod(function, variable, upperLimit, lowerLimit);
    ui_integral->showResult->setText(QString::number(result, 'f'));
    updateHistoryList(ui_integral->lineEdit->text());
}

// перерисовывает интерфейс приложения для расчета численных значений интегралов
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

    // Создание действий (пунктов меню)
    QAction* setDefault = new QAction("Default calculator", menu);
    QAction* setIntegral = new QAction("Numerical Integral Calculator", menu);

    // Добавление действий в меню
    menu->addAction(setDefault);
    menu->addAction(setIntegral);
    // Установка меню для QToolButton
    ui_integral->menuChoice->setMenu(menu);
    // Установка стиля всплывающего меню
    ui_integral->menuChoice->setPopupMode(QToolButton::InstantPopup);

    connect(setDefault, &QAction::triggered, this, &VisualCalculator::loadStandardInterface);
    connect(setIntegral, &QAction::triggered, this, &VisualCalculator::updateInterfaceIntegral);

    connect(ui_integral->lineEdit, &QLineEdit::textChanged, this, &VisualCalculator::updateDisplayText);
    connect(ui_integral->equalBtn, &QPushButton::clicked, this, &VisualCalculator::calculateResult);
    connect(ui_integral->copyBtn, &QPushButton::clicked, this, &VisualCalculator::copyExpressionToClipboard);
    connect(ui_integral->clearBtn, &QPushButton::clicked, this, &VisualCalculator::clearExpression);
    connect(ui_integral->lineEdit, &QLineEdit::returnPressed, this, &VisualCalculator::handleEnterPressed);
    connect(ui_integral->historyList, &QListWidget::itemDoubleClicked, this, &VisualCalculator::historyListDoubleClicked);
}

// рисует стандартный интефейс - обычный калькулятор
void VisualCalculator::loadStandardInterface()
{
    generateStandartInterface();
}

