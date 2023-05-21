#include "visualcalculator.h"
#include <QClipboard>
#include <ui_visualcalculator.h>

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

bool isValidInput(const QString& input)
{
    QRegularExpression validRegex("^[0-9()+\\-*/. ]+$");
    QRegularExpressionMatch match = validRegex.match(input);
    return match.hasMatch();
}

bool isOperator(const QString& token)
{
    return token == "+" || token == "-" || token == "*" || token == "/";
}
// Добавляет недостающие закрывающие скобки
void autoBalanceParentheses(QString& expression)
{
    QStack<QChar> parenthesesStack;

    for (int i = 0; i < expression.length(); ++i) {
        QChar ch = expression.at(i);

        if (ch == '(') {
            // Открывающая скобка, добавляем ее в стек
            parenthesesStack.push(ch);
        }
        else if (ch == ')') {
            // Закрывающая скобка

            if (parenthesesStack.isEmpty()) {
                // Нет соответствующей открывающей скобки, удаляем закрывающую скобку
                expression.remove(i, 1);
                --i;
            }
            else {
                // Есть соответствующая открывающая скобка, удаляем ее из стека
                parenthesesStack.pop();
            }
        }
    }

    // Добавляем недостающие закрывающие скобки
    while (!parenthesesStack.isEmpty()) {
        expression.append(')');
        parenthesesStack.pop();
    }
}

// расчитываете выражение с одним операндом
double performOperation(double operand1, double operand2, const QString& operation)
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
        if (operand2 == 0) {
            QMessageBox::warning(nullptr, "Warning", "You divide by zero");
        }
        return operand1 / operand2;
    }



    else {
        // Неподдерживаемая операция
        return std::numeric_limits<double>::quiet_NaN();
    }
}

// перевод обычной строки в обратную польскую нотацию
QStringList convertToRPN(const QString& expression) {

    QStringList outputQueue;  // Очередь для выходной обратной польской записи
    QStack<QString> operatorStack;  // Стек для операторов

    QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);

    for (const QString& token : tokens) {
        if (!isOperator(token) && token != "(" && token != ")") {
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
        else if (token == "(") {
            // Текущий токен - открывающая скобка, добавляем ее в стек операторов
            operatorStack.push(token);
        }
        else if (token == ")") {
            // Текущий токен - закрывающая скобка

            // Перемещаем операторы из стека в выходную очередь до тех пор,
            // пока не встретим соответствующую открывающую скобку или стек не опустеет
            while (!operatorStack.isEmpty() && operatorStack.top() != "(") {
                outputQueue.append(operatorStack.pop());
            }

            // Если стек опустел и не было найдено открывающей скобки, значит в выражении скобки не сбалансированы
            if (operatorStack.isEmpty()) {
                QMessageBox::warning(nullptr, "Warning", "Unbalanced parentheses");
                return QStringList();  // Возвращаем пустой список, чтобы обозначить ошибку
            }

            // Удаляем открывающую скобку из стека
            operatorStack.pop();
        }
        else {
            // Текущий токен - оператор

            while (!operatorStack.isEmpty() && operatorStack.top() != "(" &&
                getOperatorPrecedence(token) <= getOperatorPrecedence(operatorStack.top())) {
                // Текущий оператор имеет меньший или равный приоритет, чем оператор на вершине стека
                // Перемещаем оператор с вершины стека в выходную очередь
                outputQueue.append(operatorStack.pop());
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

double calculateExpression(const QString& expression)
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

            // Если введён только операнд без чисел возвращаем 0
            if (numbersStack.isEmpty()) return 0.0;

            double operand2 = numbersStack.pop();
            double operand1 = numbersStack.pop();
            double result = performOperation(operand1, operand2, token);
            numbersStack.push(result);
        }
    }

    return numbersStack.pop();
}

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
