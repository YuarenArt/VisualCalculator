#include "visualcalculator.h"
#include <QClipboard>
#include <ui_visualcalculator.h>

//you need to connect a third-party project with the CalculatorUtils namespace(https://github.com/YuarenArt/CalculatorUtils), 
//which defines functions for calculations and transformations that do not depend on the Visualcalculator class

#include "\QT Lib\CalculatorUtils\CalculatorUtils.cpp"
using namespace CalculatorUtils;

VisualCalculator::VisualCalculator(QWidget *parent) : QMainWindow(parent), ui(new Ui::VisualCalculator)
{
    ui->setupUi(this);

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

void VisualCalculator::updateHistoryList(const QString& text)
{
    ui->historyList->addItem(text);
}

void VisualCalculator::clearExpression()
{
    ui->lineEdit->clear();
    calculateResult();
}

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
    QRegularExpression spaceRegex("(\\d)([+\\-*/^sqrtloglnsincostancottgctg])");
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
    formattedText.replace(QRegularExpression("([+\\-*/^sqrtloglnsincostancottgctg])(\\d+)"), "\\1 \\2");
    formattedText.replace(QRegularExpression("(\\d+)\\s+([+\\-*/^sqrtloglnsincostancottgctg])"), "\\1 \\2");
    formattedText.replace(QRegularExpression("([+\\-*/]^sqrtloglnsincostancottgctg)\\s*(\\d+)"), "\\1 \\2");


    formattedText.replace(QRegularExpression(",") , ".");

    /*formattedText.replace(QRegularExpression("(cos)\\s+(\\d)"), "\\1\\2");
    formattedText.replace(QRegularExpression("(sqrt)\\s+(\\d)"), "\\1\\2");*/

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

void VisualCalculator::historyListDoubleClicked()
{
    QListWidgetItem* item = ui->historyList->currentItem();
    ui->lineEdit->setText(item->text());
}
