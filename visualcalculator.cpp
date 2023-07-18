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

    // Создание действий (пунктов меню)
    QAction* setDefault = new QAction("Default calculator", menu);
    QAction* setIntegral = new QAction("Numerical Integral Calculator", menu);

    // Добавление действий в меню
    menu->addAction(setDefault);
    menu->addAction(setIntegral);
    // Установка меню для QToolButton
    ui->menuChoice->setMenu(menu);
    // Установка стиля всплывающего меню
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

// функция добавляет прошлые выражения в список 
void VisualCalculator::updateHistoryList(const QString& text)
{
    if (ui) {
        ui->historyList->addItem(text);
    }

    else {
        ui_integral->historyList->addItem(text);
    }
}

// очищает введенное выражение
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

        // Проверяем пустой ввод
        if (expression.isEmpty())
        {
            // Очищаем результат
            ui->showResult->setText(" ");
            return;
        }
        // вычисляем выражение
        double result = calculateExpression(expression);

        updateHistoryList(expression);

        // Обновляем текст в showResult
        ui->showResult->setText(QString::number(result));
    }
    else {
        QString expression = ui_integral->textShow->toPlainText();

        // Проверяем пустой ввод
        if (expression.isEmpty())
        {
            // Очищаем результат
            ui_integral->showResult->setText(" ");
            return;
        }
        // вычисляем выражение
        double result = calculateExpression(expression);

        updateHistoryList(expression);

        // Обновляем текст в showResult
        ui_integral->showResult->setText(QString::number(result));
    }
}

// выводит введенное пользователем выражение

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

        // Добавляем недостающие закрывающие скобки
        updateText(formattedText);

        formattedText.replace(QRegularExpression("(\\d+)(\\w+)"), "\\1 * \\2");
        formattedText.replace(QRegularExpression("(\\w+)([+\\-*/^lgsqrtloglnsincostancottgctg])"), "\\1 \\2");

        // Обновляем текст в textShow
        ui_integral->textShow->setText(formattedText);
    }

}

// Слот копирует выражение в буфер обмена
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

// Слот для обработки события нажатия клавиши Enter в lineEdit
void VisualCalculator::handleEnterPressed()
{
    calculateResult();
}

// при двоном щедчке по спику выражений в листе истории, добавляет выбранное выражение в lineEdit
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

// расчет введенного интеграла
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
    connect(ui_integral->equalBtn, &QPushButton::clicked, this, &VisualCalculator::calculateResultIntegral);
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

