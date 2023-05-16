#include "visualcalculator.h"
#include <QStack>

int getOperatorPrecedence(const QString& op) {
    if (op == "+" || op == "-") {
        return 1;
    }
    else if (op == "*" || op == "/") {
        return 2;
    }
    else {
        // Неизвестный оператор, можно вернуть 3 или другое значение по умолчанию
        return 3;
    }
}

VisualCalculator::VisualCalculator(QWidget *parent) : QMainWindow(parent)
{

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    //основной вертикальный виджет куда всё размещается
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);


    QHBoxLayout* inputLayout = new QHBoxLayout;
    mainLayout->addLayout(inputLayout);

    // создания поля ввода
    lineEdit = new QLineEdit(centralWidget);
    inputLayout->addWidget(lineEdit);


    // Создание кнопки "="
    equalBtn = new QPushButton("=");
    inputLayout->addWidget(equalBtn);

    showResult = new QLineEdit(centralWidget);
    showResult->setReadOnly(true);
    mainLayout->addWidget(showResult);

    // показывает что было введено пользователем
    textShow = new QTextEdit(centralWidget);
    textShow->setReadOnly(true);
    mainLayout->addWidget(textShow);

    // Установка размера шрифта
    QFont font = textShow->font();
    font.setPointSize(font.pointSize() + 12);
    textShow->setFont(font);

    connect(lineEdit, &QLineEdit::textChanged, this, &VisualCalculator::updateTextShow);
    connect(equalBtn, &QPushButton::clicked, this, &VisualCalculator::calculate);
}

bool VisualCalculator::isValidInput(const QString& input)
{
    QRegularExpression validRegex("^[0-9()+\\-*/. ]+$");
    QRegularExpressionMatch match = validRegex.match(input);
    return match.hasMatch();
}

bool VisualCalculator::isOperator(const QString& token)
{
    return token == "+" || token == "-" || token == "*" || token == "/";
}

void VisualCalculator::calculate()
{
    QString expression = textShow->toPlainText();

    // Проверяем пустой ввод
    if (expression.isEmpty())
    {
        // Очищаем результат
        showResult->setText(" ");
        return;
    }

    double result = calculateExpression(expression);

    // Обновляем текст в showResult
    showResult->setText(QString::number(result));
}

double VisualCalculator::calculateExpression(const QString& expression)
{
    // Создание списка токенов (числа и операторы)
    QStringList tokens = convertToRPN(expression);

    if (tokens.isEmpty())
    {
        return 0;
    }

    // Заполняем оба стека
    QStack<double> numbersStack;

    for (const QString& token : tokens) {
        if (!isOperator(token)) {
            // Текущий токен - число
            numbersStack.push(token.toDouble());
        }
        else {
            // Текущий токен - оператор
            double operand2 = numbersStack.pop();
            double operand1 = numbersStack.pop();
            double result = performOperation(operand1, operand2, token);
            numbersStack.push(result);
        }
    }

    return numbersStack.pop();
}

QStringList VisualCalculator::convertToRPN(const QString& expression) {

    QStringList outputQueue;  // Очередь для выходной обратной польской записи
    QStack<QString> operatorStack;  // Стек для операторов

    QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);

    for (const QString& token : tokens) {
        if (!isOperator(token)) {
            // Проверяем, является ли токен числом
            bool isNumber;
            token.toDouble(&isNumber);
            if (isNumber) {
                // Текущий токен - число, добавляем его в выходную очередь
                outputQueue.append(token);
            }
            else {
                // Обработка ошибки - токен не является числом
                QMessageBox::warning(nullptr, "Warning", "Unknown operand");
            }
        }
        else {
            // Текущий токен - оператор
            while (!operatorStack.isEmpty() && isOperator(operatorStack.top())) {

                QString topOperator = operatorStack.top();
                if (getOperatorPrecedence(token) <= getOperatorPrecedence(topOperator)) {
                    // Текущий оператор имеет меньший или равный приоритет, чем оператор на вершине стека
                    // Перемещаем оператор с вершины стека в выходную очередь
                    outputQueue.append(operatorStack.pop());
                }
                else {
                    break;
                }
            }
            // Добавляем текущий оператор в стек
            operatorStack.push(token);
        }
    }

    // Перемещаем оставшиеся операторы из стека в выходную очередь
    while (!operatorStack.isEmpty()) {
        outputQueue.append(operatorStack.pop());
    }

    return outputQueue;
}

double VisualCalculator::performOperation(double operand1, double operand2, const QString& operation)
{
    if (operation == "+") {
        return operand1 + operand2;
    }
    else if (operation == "-") {
        return operand1 - operand2;
    }
    else if (operation == "*") {
        return operand1 * operand2;
    }
    else if (operation == "/") {
        if (operand2 == 0)   {
            QMessageBox::warning(nullptr, "Warning", "You divide by zero");
        }
        return operand1 / operand2;
    }
    else {
        // Неподдерживаемая операция
        return std::numeric_limits<double>::quiet_NaN();
    }
}

void VisualCalculator::updateTextShow(const QString& text)
{
    QString formattedText = text;

    // Добавляем пробелы между числами и операндами, если они отсутствуют
    QRegularExpression spaceRegex("(\\d)([+\\-*/])");
    formattedText.replace(spaceRegex, "\\1 \\2 ");
    formattedText.replace("(", "( ");
    formattedText.replace(")", " )");

   

    // Обновляем текст в textShow
    textShow->setText(formattedText);
}

