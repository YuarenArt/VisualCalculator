#include "visualcalculator.h"
#include <QClipboard>
#include <ui_visualcalculator.h>
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
}

VisualCalculator::~VisualCalculator()
{
    delete ui;
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

    double result = calculateExpression(expression);

    // Обновляем текст в showResult
    ui->showResult->setText(QString::number(result));
}


void VisualCalculator::updateDisplayText(const QString& text)
{
    QString formattedText = text;

    // Добавляем недостающие закрывающие скобки
    int openBracketsCount = formattedText.count("(");
    int closeBracketsCount = formattedText.count(")");
    if (openBracketsCount > closeBracketsCount) {
        formattedText.append(") ");
    }

    // Удаляем закрывающую скобку, если есть только закрывающая, без открытой
    if (closeBracketsCount > openBracketsCount) {
        formattedText.chop(1);
    }

    // Добавляем пробелы между числами и операндами, если они отсутствуют
    QRegularExpression spaceRegex("(\\d)([+\\-*/])");
    formattedText.replace(spaceRegex, "\\1 \\2 ");

    // Добавляем пробелы перед и после скобок
    formattedText.replace(QRegularExpression("\\("), " ( ");
    formattedText.replace(QRegularExpression("\\)"), " ) ");

    // Добавляем знак умножения, если между числом и скобкой есть пробел
    QRegularExpression numberBracketRegex("(\\d) \\(");
    formattedText.replace(numberBracketRegex, "\\1 * (");
    QRegularExpression bracketNumberRegex("\\) (\\d)");
    formattedText.replace(bracketNumberRegex, ") * \\1");

    // Добавляем знак умножения, если между скобками нет знака операции
    formattedText.replace(QRegularExpression("\\)\\s*\\("), ") * (");

    // Удаляем лишние пробелы между числами
    QRegularExpression numberRegex("(\\d) (\\d)");
    formattedText.replace(numberRegex, "\\1\\2");

    // Добавляем пробел после операнда, который следует после закрывающей скобки
    formattedText.replace(QRegularExpression("\\)([+\\-*/])"), ") \\1\\");

    // Объединяем числа, если между ними есть пробел, но нет операнда
    formattedText.replace(QRegularExpression("(\\d) + (\\d)"), "\\1\\2");

    // Обновляем текст в textShow
    ui->textShow->setText(formattedText);
}

// Слот копирует выражение в буфер обмена
void VisualCalculator::copyExpressionToClipboard()
{
    QString expression = ui->showResult->text();

    QApplication::clipboard()->setText(expression);
}

// Слот для обработки события нажатия клавиши Enter
void VisualCalculator::handleEnterPressed()
{
    calculateResult();
}
