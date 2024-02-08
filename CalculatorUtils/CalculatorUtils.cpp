#include <QMessageBox>
#include <QStack>

#include "CalculatorUtils.h"
#include "stringRefactor.h"



// constant 
namespace CalculatorUtils {
    const QString OPERATORSWITHFUNCTIONS = "[+\\-*/.^lgsqrtloglnsincostancottgctg]";
    const QString OPERATORS = "[+\\-*/^]";
    const QString ALLCURRENTSIMBOL = "^[0-9()+\\-*/.^lgsqrtloglnsincostancottgctg ]+$";
    const QString FUNCTIONS = "[lgsqrtloglnsincostancottgctg]";

    static const QRegularExpression validRegex(ALLCURRENTSIMBOL);
    static const QRegularExpression operatorsRegex(OPERATORS);
    static const QRegularExpression functionsRegex(FUNCTIONS);
    static const QRegularExpression negativeNumberRegex("-\\d+");
}

namespace CalculatorUtils {

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
            // ����������� ��������, ����� ������� 1 ��� ������ �������� �� ���������
            return 1;
        }
    }

    bool isValidInput(const QString& input)
    {
        QRegularExpressionMatch match = validRegex.match(input);
        return match.hasMatch();
    }

    bool isOperator(const QString& input)
    {
        if (operatorsRegex.match(input).hasMatch() && operatorsRegex.match(input).captured() == input) {
            return true;
        }
        return functionsRegex.match(input).hasMatch();
    }

    bool isNegativeNumber(const QString& input)
    {
        QRegularExpressionMatch match = negativeNumberRegex.match(input);
        return match.hasMatch();
    }

    bool isOperatorWithOneOperand(const QString& input)
    {
        QRegularExpressionMatch match = functionsRegex.match(input);
        return match.hasMatch();
    }

    // ��������� ����������� ����������� ������
    QString autoBalanceParentheses(QString& expression)
    {
        QStack<QChar> parenthesesStack;

        QString newExpression = expression;

        for (int i = 0; i < newExpression.length(); ++i) {
            QChar ch = newExpression.at(i);

            if (ch == '(') {
                // ����������� ������, ��������� �� � ����
                parenthesesStack.push(ch);
            }
            else if (ch == ')') {
                // ����������� ������

                if (parenthesesStack.isEmpty()) {
                    // ��� ��������������� ����������� ������, ������� ����������� ������
                    newExpression.remove(i, 1);
                    --i;
                }
                else {
                    // ���� ��������������� ����������� ������, ������� �� �� �����
                    parenthesesStack.pop();
                }
            }
        }

        // ��������� ����������� ����������� ������
        while (!parenthesesStack.isEmpty()) {
            newExpression.append(')');
            parenthesesStack.pop();
        }

        return newExpression;
    }

    // ������������ ���������
    double performOperation(double operand1, const QString& operation, double operand2) {
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
            if (operand2 == 0) {
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

        // ���������������� ��������
        return std::numeric_limits<double>::quiet_NaN();
    } 

    QStringList convertToRPN(const QString& expression) {

        QStringList outputQueue;  // ������� ��� �������� �������� �������� ������
        QStack<QString> operatorStack;  // ���� ��� ����������

        QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);

        for (const QString& token : tokens) {
            if ((!isOperator(token) || isNegativeNumber(token)) && token != "(" && token != ")") {
                // ���������, �������� �� ����� ������
                bool isNumber;
                token.toDouble(&isNumber);
                if (isNumber) {
                    // ������� ����� - �����, ��������� ��� � �������� �������
                    outputQueue.append(token);
                }
                else {
                    // ��������� ������ - ����� �� �������� ������
                    QMessageBox::warning(nullptr, "Warning", "Unknown operand");
                }
            }
            else if (token == "(") {
                // ������� ����� - ����������� ������, ��������� �� � ���� ����������
                operatorStack.push(token);
            }
            else if (token == ")") {
                // ������� ����� - ����������� ������

                // ���������� ��������� �� ����� � �������� ������� �� ��� ���,
                // ���� �� �������� ��������������� ����������� ������ ��� ���� �� ��������
                while (!operatorStack.isEmpty() && operatorStack.top() != "(") {
                    outputQueue.append(operatorStack.pop());
                }

                // ���� ���� ������� � �� ���� ������� ����������� ������, ������ � ��������� ������ �� ��������������
                if (operatorStack.isEmpty()) {
                    QMessageBox::warning(nullptr, "Warning", "Unbalanced parentheses");
                    return QStringList();  // ���������� ������ ������, ����� ���������� ������
                }

                // ������� ����������� ������ �� �����
                operatorStack.pop();
            }
            else {
                // ������� ����� - ��������

                while (!operatorStack.isEmpty() && operatorStack.top() != "(" &&
                    getOperatorPrecedence(token) <= getOperatorPrecedence(operatorStack.top())) {
                    // ������� �������� ����� ������� ��� ������ ���������, ��� �������� �� ������� �����
                    // ���������� �������� � ������� ����� � �������� �������
                    outputQueue.append(operatorStack.pop());
                }

                // ��������� ������� �������� � ����
                operatorStack.push(token);
            }
        }

        // ���������� ���������� ��������� �� ����� � �������� �������
        while (!operatorStack.isEmpty()) {
            outputQueue.append(operatorStack.pop());
        }

        return outputQueue;
    }

    double calculateExpressionWithRPN(const QString& expression)
    {
        if (!isValidInput(expression)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // �������� ������ ������� (����� � ���������)
        QStringList tokens = convertToRPN(expression);

        if (tokens.isEmpty())
        {
            return 0.0;
        }

        // ��������� ��� �����
        QStack<double> numbersStack;

        for (const QString& token : tokens) {


            if (!isOperator(token) || isNegativeNumber(token)) {
                // ������� ����� - �����
                numbersStack.push(token.toDouble());
            }
            else {
                // ������� ����� - ��������

                // ���� ����� ��� � �� ����� � �� �������� ������������ ������
                if (!isOperator(token)) {
                    // ������� ��������� �� ������ ������������
                    qDebug() << "������: ������������ ���� - " << token;
                    // ������� "nan" ��� ������ ����������� ��������, ����� ���������� ������
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

    QString updateStandartText(const QString& text)
    {
        QString formattedText = text;
        formattedText = autoBalanceParentheses(formattedText);

        StringRefactor refactoredString(formattedText);
        refactoredString.updateStandardText();
        return refactoredString.getFormattedText();
    }

    QString updateIntegralText(const QString& text, const QString& variable)
    {
        QString formattedText = text;
        formattedText = autoBalanceParentheses(formattedText);

        StringRefactor refactoredString(formattedText);
        refactoredString.updateIntegralText(variable);
        return refactoredString.getFormattedText();
    }

    double calculateExpressionWithVariable(const QString& expression, const QString& variable, double variableValue)
    {

        QString variableValueString = QString::number(variableValue);
        QString substitutedExpression = expression;
        substitutedExpression.replace(variable, variableValueString);

        if (!isValidInput(substitutedExpression)) {

            return std::numeric_limits<double>::quiet_NaN();
        }

        return calculateExpressionWithRPN(substitutedExpression);
    }

    double calculateIntegralSimpsonsMethod(QString&  function, const QString& variable, const double& upperLimit, const double& lowerLimit, const qint64& n)
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

    StringRefactor::StringRefactor(QString text) : formattedText(text) {
        regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    }

    void StringRefactor::updateStandardText()
    {
        
        addSpacesBetweenNumbersAndOperators();
        addMultiplicationOperator();
        addSpacesAroundBrackets();
        removeExtraSpacesBetweenNumbers();
        removeSpaceBetweenOperatorsAndNumbers();
        replaceCommasWithPeriods();
        removeExtraSpaces();
        addSpacesAfterFuntionAndOperators();
    }

    void StringRefactor::updateIntegralText(const QString& variable)
    {
        addSpacesBetweenNumbersAndOperators();
        addMultiplicationOperator();
        addSpacesAroundBrackets();
        removeExtraSpacesBetweenNumbers();
        removeSpaceBetweenOperatorsAndNumbers();
        replaceCommasWithPeriods();
        removeExtraSpaces();
        addSpacesAfterFuntionAndOperators();

        ignoreVariableInFormatting(variable);
    }

    QString StringRefactor::getFormattedText()
    {
        return formattedText;
    }

    void StringRefactor::addSpacesBetweenNumbersAndOperators()
    {
        regex.setPattern("(\\d+)\\s*([+\\-*/^sqrtloglnlgsincostancottgctg])");
        formattedText.replace(regex, "\\1 \\2");
    }

    void StringRefactor::addSpacesAroundBrackets()
    {
        regex.setPattern("\\(");
        formattedText.replace(regex, " ( ");
        regex.setPattern("\\)");
        formattedText.replace(regex, " ) ");
    }

    void StringRefactor::addMultiplicationOperator()
    {
        regex.setPattern("(\\d) *\\(");
        formattedText.replace(regex, "\\1 * (");

        regex.setPattern("\\) (\\d)");
        formattedText.replace(regex, ") * \\1 ");

        regex.setPattern("\\)\\s*\\(");
        formattedText.replace(regex, ") * (");

        regex.setPattern("( \\) )\\s*([lgsqrtloglnsincostancottgctg])");
        formattedText.replace(regex, "\\1 * \\2");

        regex.setPattern("(\\d+)\\s*([lgsqrtloglnsincostancottgctg])");
        formattedText.replace(regex, "\\1 * \\2");
    }

    void StringRefactor::removeExtraSpacesBetweenNumbers()
    {
        regex.setPattern("(\\d+)\\s*(\\d+)");
        formattedText.replace(regex, "\\1\\2");
    }

    void StringRefactor::removeSpaceBetweenOperatorsAndNumbers()
    {
        regex.setPattern("([+\\-*/sqrtloglnsincostanlgcottgctg(]) (\\d+)");
        formattedText.replace(regex, "\\1 \\2");
        regex.setPattern("(\\d+)\\s+([+\\-*/sqrtloglnsinlgcostancottgctg])");
        formattedText.replace(regex, "\\1 \\2");
        regex.setPattern("([+\\-*/]sqrtloglnsincostanlgcottgctg)\\s*(\\d+)");
        formattedText.replace(regex, "\\1 \\2");
        regex.setPattern("([cossintgtancthcotloglnsqrtlg^])\\s+(\\d+)");
        formattedText.replace(regex, "\\1 ( \\2 )");
    }

    void StringRefactor::replaceCommasWithPeriods()
    {
        regex.setPattern(",");
        formattedText.replace(regex, ".");
    }

    void StringRefactor::removeExtraSpaces()
    {
        regex.setPattern("\\s+");
        formattedText.replace(regex, " ");
    }

    void StringRefactor::addSpacesAfterFuntionAndOperators()
    {
        // Add parentheses around single-digit numbers
        regex.setPattern( "([lgsqrtloglnsincostancottgctg])(\\d+)");
        formattedText.replace(regex, "\\1 ( \\2 ) ");

        regex.setPattern("("+ OPERATORS + ")\\s*(\\d+)");
        formattedText.replace(regex, "\\1 \\2");
    }

    void StringRefactor::ignoreVariableInFormatting(const QString variable)
    {
        // ������� ������� ������ ���������� � ��������� �� � ���������� ���������
        QString variablePattern = "\\b" + variable.trimmed() + "\\b";

        // ���������� ���������� ��������� � ����������� ����������
        QRegularExpression regex(variablePattern + "(\\d+)");
        formattedText.replace(regex, "\\1");


    }
}

