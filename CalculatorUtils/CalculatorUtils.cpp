#include <QMessageBox>
#include <QStack>

#include "stringRefactor.h"

namespace CalculatorUtils {

    // Шаблон для операндов, которые могут использоваться в выражении.
    // Содержит символы [+\\-*/.^lgsqrtloglnsincostancottgctg].
    // Знак минус в методе isOperator он принимает отрицательное число за операнд
    // Поэтому проверка минуса и отрицательного числа вынесена как отдельная функция isNegativeNumber
    const QString OPERATORSWITHFUNCTIONS = "[+\\-*/^lgsqrtloglnsincostancottgctg]";

    const QString OPERATORS = "[+\\-*/^]";

    // Шаблон для проверки правильности всех символов в текущем выражении.
    // Может содержать только цифры, скобки, арифметические операторы и функции.
    const QString ALLCURRENTSIMBOL = "^[0-9()+\\-*/.^lgsqrtloglnsincostancottgctg ]+$";

    // Шаблон для функций которые требуют только один аргумент
    const QString FUNCTIONS = "[lgsqrtloglnsincostancottgctg]";

    int getOperatorPrecedence(const QString& op) {
        if (op == "^" || op == "sqrt" || op == "log" || op == "ln" || op == "sin" || op == "cos" || op == "tan" || op == "cot" || op == "tg" || op == "ctg") {
            return 4;
        }
        else if (op == "*" || op == "/") {
            return 3;
        }
        else if (op == "+" || op == "-") {
            return 2;
        }
        else {
            // Неизвестный оператор, можно вернуть 1 или другое значение по умолчанию
            return 1;
        }
    }

    bool isValidInput(const QString& input)
    {
        QRegularExpression validRegex(ALLCURRENTSIMBOL);
        QRegularExpressionMatch match = validRegex.match(input);
        return match.hasMatch();
    }

    bool isOperator(const QString& input)
    {
        QRegularExpression operatorsRegex(OPERATORS);
        QRegularExpression functionsRegex(FUNCTIONS);

        if (operatorsRegex.match(input).hasMatch() && operatorsRegex.match(input).captured() == input) {
            return true;
        }

        return functionsRegex.match(input).hasMatch();

    }

    bool isNegativeNumber(const QString& input)
    {
        QRegularExpression validRegex("-\\d+");
        QRegularExpressionMatch match = validRegex.match(input);
        bool isMinusBeforeNumber = match.hasMatch();
        return isMinusBeforeNumber;
    }

    bool isOperatorWithOneOperand(const QString& input)
    {
        QRegularExpression validRegex(FUNCTIONS);
        QRegularExpressionMatch match = validRegex.match(input);
        return match.hasMatch();
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

    // расчитываете выражение
    double performOperation(double operand1, const QString& operation, double operand2 = 0) {
        if (operation == "+") {
            return operand1 + operand2;
        }
        if (operation == "-") {
            return operand1 - operand2;
        }
        if (operation == "*") {
            return operand1 * operand2;
        }
        if (operation == "/") {
            if (operand1 == 0) {
                QMessageBox::warning(nullptr, "Warning", "You divide by zero");
            }
            return operand1 / operand2;
        }
        if (operation == "^") {
            return pow(operand1, operand2);
        }
        if (operation == "sqrt") {
            return sqrt(operand1);
        }
        if (operation == "log") {
            return log10(operand1);
        }
        if (operation == "ln" || operation == "lg") {
            return log(operand1);
        }
        if (operation == "sin") {
            return sin(operand1);
        }
        if (operation == "cos") {
            return cos(operand1);
        }
        if (operation == "tan" || operation == "tg") {
            return tan(operand1);
        }
        if (operation == "cot" || operation == "ctg") {
            return 1 / tan(operand1);
        }

        // Неподдерживаемая операция
        return std::numeric_limits<double>::quiet_NaN();
    } 

    QStringList convertToRPN(const QString& expression) {

        QStringList outputQueue;  // Очередь для выходной обратной польской записи
        QStack<QString> operatorStack;  // Стек для операторов

        QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);

        for (const QString& token : tokens) {
            if ((!isOperator(token) || isNegativeNumber(token)) && token != "(" && token != ")") {
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

    double calculateExpressionWithRPN(const QString& expression)
    {
        // Создание списка токенов (числа и операторы)
        QStringList tokens = convertToRPN(expression);

        if (tokens.isEmpty())
        {
            return 0.0;
        }

        // Заполняем оба стека
        QStack<double> numbersStack;

        for (const QString& token : tokens) {


            if (!isOperator(token) || isNegativeNumber(token)) {
                // Текущий токен - число
                numbersStack.push(token.toDouble());
            }
            else {
                // Текущий токен - оператор

                // Если токен это и не число и не оператор обрабатываем ошибку
                if (!isOperator(token)) {
                    // Вывести сообщение об ошибке пользователю
                    qDebug() << "Ошибка: Некорректный ввод - " << token;
                    // Вернуть "nan" или другое специальное значение, чтобы обозначить ошибку
                    return std::numeric_limits<double>::quiet_NaN();
                }

                if (isOperatorWithOneOperand(token)) {
                    double operand1 = numbersStack.pop();
                    double result = performOperation(operand1, token);
                    numbersStack.push(result);
                    continue;
                }

                double operand2 = numbersStack.pop();
                double operand1 = 0;
                if(!(numbersStack.isEmpty())) {
                    operand1 = numbersStack.pop();
                }

                double result = performOperation(operand1, token, operand2);
                numbersStack.push(result);

            }
        }

        return numbersStack.pop();
    }

    void updateText(QString& formattedText)
    {
        autoBalanceParentheses(formattedText);

        StringRefactor refactoredString(formattedText);
        refactoredString.updateStandartText();
    }

    double calculateExpressionWithVariable(const QString& expression, const QString& variable, double variableValue)
    {
        QString variableValueString = QString::number(variableValue);
        QString substitutedExpression = expression;
        substitutedExpression.replace(variable, variableValueString);

        return calculateExpressionWithRPN(substitutedExpression);
    }

    double calculateIntegralSimpsonsMethod(QString&  function, const QString& variable, const double& upperLimit, const double& lowerLimit, const qint64& n = 1000)
    {

        double h = (upperLimit - lowerLimit) / n;
        double sum = calculateExpressionWithVariable(function, variable, lowerLimit) + calculateExpressionWithVariable(function, variable, upperLimit);

        for (size_t i = 1; i < n; i += 2) {
            double x = lowerLimit + i * h;
            sum += 4 * calculateExpressionWithVariable(function, variable, x);
        }

        for (size_t i = 2; i < n - 1; i += 2) {
            double x = lowerLimit + i * h;
            sum += 2 * calculateExpressionWithVariable(function, variable, x);
        }

        double result = sum * h / 3.0;
        return result;

    }

} 

// stringRefactor methods
namespace CalculatorUtils {

    StringRefactor::StringRefactor(QString text) : formattedText(text) {}

    void StringRefactor::updateStandartText()
    {
        addSpacesBetweenNumbersAndOperators();
        addSpacesAroundBrackets();
        addMultiplicationOperator();
        removeExtraSpacesBetweenNumbers();
        removeSpaceBetweenOperatorsAndNumbers();
        formatNumbersWithOperators();
        removeLeadingZerosAndNormalizeDecimals();
        removeExtraDecimals();
        replaceCommasWithPeriods();
        handleMinusSigns();
        removeExtraSpaces();
    }

    QString StringRefactor::getFormattedText()
    {
        return formattedText;
    }

    void StringRefactor::addSpacesBetweenNumbersAndOperators()
    {
        QRegularExpression spaceRegex("(\\d)([+\\-*/^sqrtloglnlgsincostancottgctg])");
        formattedText.replace(spaceRegex, "\\1 \\2 ");
    }

    void StringRefactor::addSpacesAroundBrackets()
    {
        formattedText.replace(QRegularExpression("\\("), " ( ");
        formattedText.replace(QRegularExpression("\\)"), " ) ");
    }

    void StringRefactor::addMultiplicationOperator()
    {
        QRegularExpression numberBracketRegex("(\\d) *\\(");
        formattedText.replace(numberBracketRegex, "\\1 * (");
        QRegularExpression bracketNumberRegex("\\) (\\d)");
        formattedText.replace(bracketNumberRegex, ") * \\1 ");
        formattedText.replace(QRegularExpression("\\)\\s*\\("), ") * (");
    }

    void StringRefactor::removeExtraSpacesBetweenNumbers()
    {
        QRegularExpression numberRegex("(\\d+)\\s*(\\d+)");
        formattedText.replace(numberRegex, "\\1\\2");
    }

    void StringRefactor::removeSpaceBetweenOperatorsAndNumbers()
    {
        formattedText.replace(QRegularExpression("([+\\-*/sqrtloglnsincostancottgctglg])(\\d+)"), "\\1 \\2");
        formattedText.replace(QRegularExpression("(\\d+)\\s+([+\\-*/sqrtloglnsinlgcostancottgctg])"), "\\1 \\2");
        formattedText.replace(QRegularExpression("([+\\-*/]sqrtloglnsincostancottgctglg)\\s*(\\d+)"), "\\1 \\2");
        formattedText.replace(QRegularExpression("([cossintgtancthcotloglnsqrtlg^])\\s+(\\d+)"), "\\1 ( \\2 )");
    }

    void StringRefactor::formatNumbersWithOperators()
    {
        formattedText.replace(QRegularExpression("^(0)(0*)(\\..*)?$"), "\\1\\3");
        formattedText.replace(QRegularExpression("(0)([1-9])(0*)"), "\\1.\\2\\3");
        formattedText.replace(QRegularExpression("(\\.)([1-9]+)(0+)(\\.)"), "\\1\\2\\3");
        formattedText.replace(QRegularExpression("(0+)(.)([1-9])"), "0\\2\\3");
    }

    void StringRefactor::removeLeadingZerosAndNormalizeDecimals()
    {
        formattedText.replace(QRegularExpression("^(0)(0*)(\\..*)?$"), "\\1\\3");
        formattedText.replace(QRegularExpression("(0)([1-9])(0*)"), "\\1.\\2\\3");
    }

    void StringRefactor::removeExtraDecimals()
    {
        formattedText.replace(QRegularExpression("(\\.)([1-9]+)(0+)(\\.)"), "\\1\\2\\3");
    }

    void StringRefactor::replaceCommasWithPeriods()
    {
        formattedText.replace(QRegularExpression(","), ".");
    }

    void StringRefactor::handleMinusSigns()
    {
        formattedText.replace(QRegularExpression("([+\\-*/sqrtloglnsincostanlgcottgctg(]) (-) (\\d+)"), "\\1 \\2\\3");
    }

    void StringRefactor::removeExtraSpaces()
    {
        formattedText.replace(QRegularExpression("\\s+"), " ");
    }

}
