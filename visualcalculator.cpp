#include "visualcalculator.h"
#include <QClipboard>
#include <ui_standartvisualcalculator.h>
#include <ui_integralvisualvalculator.h>

//you need to connect a third-party project with the CalculatorUtils namespace(https://github.com/YuarenArt/CalculatorUtils), 
//which defines functions for calculations and transformations that do not depend on the Visualcalculator class

#include "\QT Lib\CalculatorUtils\CalculatorUtils.cpp"
using namespace CalculatorUtils;

VisualCalculator::VisualCalculator(QWidget *parent) : QMainWindow(parent), ui(new Ui::standartVisualCalculator), ui_integral(nullptr)
{
    generateStandartInterface();
}

void VisualCalculator::generateStandartInterface()
{
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
}

// функция добавляет прошлые выражения в список 
void VisualCalculator::updateHistoryList(const QString& text)
{
    ui->historyList->addItem(text);
}

// очищает введенное выражение
void VisualCalculator::clearExpression()
{
    ui->lineEdit->clear();
    calculateResult();
}

//
void VisualCalculator::calculateResult()
{
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

// выводит введенное пользователем выражение
void VisualCalculator::updateDisplayText(const QString& text)
{
    QString formattedText = text;

    // Добавляем недостающие закрывающие скобки
    autoBalanceParentheses(formattedText);

    // Добавляем пробелы между числами и операндами, если они отсутствуют
    QRegularExpression spaceRegex("(\\d)([+\\-*/^sqrtloglnlgsincostancottgctg])");
    formattedText.replace(spaceRegex, "\\1 \\2 ");

    // Добавляем пробелы перед и после скобок
    formattedText.replace(QRegularExpression("\\("), " ( ");
    formattedText.replace(QRegularExpression("\\)"), " ) ");

    // Добавляем знак умножения, если между числом и скобкой есть пробел
    QRegularExpression numberBracketRegex("(\\d) *\\(");
    formattedText.replace(numberBracketRegex, "\\1 * (");
    QRegularExpression bracketNumberRegex("\\) (\\d)");
    formattedText.replace(bracketNumberRegex, ") * \\1");

    // Добавляем знак умножения, если между скобками нет знака операции
    formattedText.replace(QRegularExpression("\\)\\s*\\("), ") * (");

    // Удаляем лишние пробелы между числами
    QRegularExpression numberRegex("(\\d+)\\s*(\\d+)");
    formattedText.replace(numberRegex, "\\1\\2");

    // Удаления между операндами и числами
    formattedText.replace(QRegularExpression("([+\\-*/sqrtloglnsincostanlgcottgctg])(\\d+)"), "\\1 \\2");
    formattedText.replace(QRegularExpression("(\\d+)\\s+([+\\-*/sqrtloglnsinlgcostancottgctg])"), "\\1 \\2");
    formattedText.replace(QRegularExpression("([+\\-*/]sqrtloglnsincostancottgctglg)\\s*(\\d+)"), "\\1 \\2");

    formattedText.replace(QRegularExpression("([cossintgtancthcotloglnsqrtlg^])\\s+(\\d+)"), "\\1 ( \\2 )");

    // Замена числа на формат 0.xxx, если число начинается с нуля (021 = 0.21)
    formattedText.replace(QRegularExpression("(0)(\\d+)"), "\\1.\\2");

    // Замена ',' при вводе чисел на '.' для правильных вычислений
    formattedText.replace(QRegularExpression(",") , ".");

    // Обновляем текст в textShow
    ui->textShow->setText(formattedText);
}

// Слот копирует выражение в буфер обмена
void VisualCalculator::copyExpressionToClipboard()
{
    QString expression = ui->showResult->text();

    QApplication::clipboard()->setText(expression);
}

// Слот для обработки события нажатия клавиши Enter в lineEdit
void VisualCalculator::handleEnterPressed()
{
    calculateResult();
}

// при двоном щедчке по спику выражений в листе истории, добавляет выбранное выражение в lineEdit
void VisualCalculator::historyListDoubleClicked()
{
    QListWidgetItem* item = ui->historyList->currentItem();
    ui->lineEdit->setText(item->text());
}

// перерисовывает интерфейс приложения для расчета численных значений интегралов
void VisualCalculator::updateInterfaceIntegral()
{

    ui_integral = new Ui_integral::integralVisualCalculator;
    QWidget* integralWidget = new QWidget(this);
    ui_integral->setupUi(integralWidget);

    setCentralWidget(integralWidget);

}

// рисует стандартный интефейс - обычный калькулятор
void VisualCalculator::loadStandardInterface()
{
    QSize windowSize = size();
    generateStandartInterface();
    resize(windowSize);

}

