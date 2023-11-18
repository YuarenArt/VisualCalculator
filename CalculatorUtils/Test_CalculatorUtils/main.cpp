#include "../CalculatorUtils.cpp"
#include <QtTest>


class Test_CalculatorUtils : public QObject
{
    Q_OBJECT

private slots:

    void testGetOperatorPrecedence();
    void testGetOperatorPrecedence_data();

    void testIsValidInput_data();
    void testIsValidInput();

    void testIsOperator_data();
    void testIsOperator();

    void testIsNegativeNumber_data();
    void testIsNegativeNumber();

    void testIsOperatorWithOneOperand_data();
    void testIsOperatorWithOneOperand();


    void testAutoBalanceParentheses_data();
    void testAutoBalanceParentheses();

    void testPerformOperation_data();
    void testPerformOperation();
};

void Test_CalculatorUtils::testGetOperatorPrecedence_data()
{
    QTest::addColumn<QString>("operation");
    QTest::addColumn<int>("expectedPrecedence");

    QTest::newRow("^") << "^" << 4;
    QTest::newRow("sqrt") << "sqrt" << 4;
    QTest::newRow("log") << "log" << 4;
    QTest::newRow("ln") << "ln" << 4;
    QTest::newRow("sin") << "sin" << 4;
    QTest::newRow("cos") << "cos" << 4;
    QTest::newRow("tan") << "tan" << 4;
    QTest::newRow("cot") << "cot" << 4;
    QTest::newRow("tg") << "tg" << 4;
    QTest::newRow("ctg") << "ctg" << 4;
    QTest::newRow("*") << "*" << 3;
    QTest::newRow("/") << "/" << 3;
    QTest::newRow("+") << "+" << 2;
    QTest::newRow("-") << "-" << 2;
    QTest::newRow("unknown") << "unknown" << 1;
}
void Test_CalculatorUtils::testGetOperatorPrecedence()
{
    QFETCH(QString, operation);
    QFETCH(int, expectedPrecedence);

    QCOMPARE(CalculatorUtils::getOperatorPrecedence(operation), expectedPrecedence);
}


void Test_CalculatorUtils::testIsValidInput_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("valid input 1") << "3+5" << true;
    QTest::newRow("valid input 2") << "(2 * 3) - 7" << true;
    QTest::newRow("valid input 3") << "sin ( 45 )" << true;
    QTest::newRow("invalid input 1") << "5 + * 7av" << false;
    QTest::newRow("invalid input 2") << "(4 +! 3" << false;
}
void Test_CalculatorUtils::testIsValidInput()
{
    QFETCH(QString, input);
    QFETCH(bool, expectedResult);

    QCOMPARE(CalculatorUtils::isValidInput(input), expectedResult);
}


void Test_CalculatorUtils::testIsOperator_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("valid operator +") << "+" << true;
    QTest::newRow("valid operator -") << "-" << true;
    QTest::newRow("valid operator *") << "*" << true;
    QTest::newRow("valid operator /") << "/" << true;
    QTest::newRow("valid operator ^") << "^" << true;
    QTest::newRow("valid operator sqrt") << "sqrt" << true;
    QTest::newRow("valid operator log") << "log" << true;
    QTest::newRow("valid operator ln") << "ln" << true;
    QTest::newRow("valid operator sin") << "sin" << true;
    QTest::newRow("valid operator cos") << "cos" << true;
    QTest::newRow("valid operator tan") << "tan" << true;
    QTest::newRow("valid operator cot") << "cot" << true;
    QTest::newRow("valid operator tg") << "tg" << true;
    QTest::newRow("valid operator ctg") << "ctg" << true;
    QTest::newRow("invalid operator %") << "%" << false;
    QTest::newRow("invalid operator empty") << "" << false;
    QTest::newRow("invalid operator 1") << "0./" << false;
    QTest::newRow("invalid operator 2") << "//" << false;
    QTest::newRow("invalid operator 3") << "**" << false;
    QTest::newRow("invalid operator 4") << "--" << false;
    QTest::newRow("invalid operator 5") << "++" << false;
    QTest::newRow("invalid operator 6") << "21./" << false;

}
void Test_CalculatorUtils::testIsOperator()
{
    QFETCH(QString, input);
    QFETCH(bool, expectedResult);

    QCOMPARE(CalculatorUtils::isOperator(input), expectedResult);
}


void Test_CalculatorUtils::testIsNegativeNumber_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("negative number 1") << "-123" << true;
    QTest::newRow("negative number 2") << "-0.456" << true;
    QTest::newRow("positive number 1") << "- 0.456" << false;
    QTest::newRow("positive number 2") << "789" << false;
    QTest::newRow("invalid input 1") << "abc" << false;
    QTest::newRow("invalid input 2") << "123abc" << false;
    QTest::newRow("empty input") << "" << false;
}
void Test_CalculatorUtils::testIsNegativeNumber()
{
    QFETCH(QString, input);
    QFETCH(bool, expectedResult);

    QCOMPARE(CalculatorUtils::isNegativeNumber(input), expectedResult);
}


void Test_CalculatorUtils::testIsOperatorWithOneOperand_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("valid operator with one operand 1") << "sin" << true;
    QTest::newRow("valid operator with one operand 2") << "cos" << true;
    QTest::newRow("valid operator with one operand 3") << "tan" << true;
    QTest::newRow("valid operator with one operand 4") << "cot" << true;
    QTest::newRow("valid operator with one operand 5") << "sqrt" << true;
    QTest::newRow("valid operator with one operand 6") << "log" << true;
    QTest::newRow("valid operator with one operand 7") << "ln" << true;
    QTest::newRow("invalid operator with one operand 1") << "+" << false;
    QTest::newRow("invalid operator with one operand 2") << "-" << false;
    QTest::newRow("invalid operator with one operand 3") << "*" << false;
}
void Test_CalculatorUtils::testIsOperatorWithOneOperand()
{
    QFETCH(QString, input);
    QFETCH(bool, expectedResult);

    QCOMPARE(CalculatorUtils::isOperatorWithOneOperand(input), expectedResult);
}


void Test_CalculatorUtils::testAutoBalanceParentheses_data()
{
    QTest::addColumn<QString>("inputExpression");
    QTest::addColumn<QString>("expectedBalancedExpression");


    QTest::newRow("balanced expression 1") << "(1 + 2)" << "(1 + 2)";
    QTest::newRow("balanced expression 2") << "25 * (23 - (45" << "25 * (23 - (45))";
    QTest::newRow("missing closing parenthesis") << "(1 + 2" << "(1 + 2)";
    QTest::newRow("missing opening parenthesis") << "1 + 2)" << "1 + 2";
    QTest::newRow("multiple missing opening parentheses") << "1 + 2))" << "1 + 2";
    QTest::newRow("multiple missing closing parentheses") << "((1 + 2" << "((1 + 2))";
}
void Test_CalculatorUtils::testAutoBalanceParentheses()
{
    QFETCH(QString, inputExpression);
    QFETCH(QString, expectedBalancedExpression);

    QString expression = inputExpression;
    CalculatorUtils::autoBalanceParentheses(expression);

    QCOMPARE(expression, expectedBalancedExpression);
}


void Test_CalculatorUtils::testPerformOperation_data()
{
    QTest::addColumn<double>("operand1");
    QTest::addColumn<QString>("operation");
    QTest::addColumn<double>("operand2");
    QTest::addColumn<double>("expectedResult");

    // Add test cases
    QTest::newRow("Addition") << 5.0 << "+" << 3.0 << 8.0;
    QTest::newRow("Subtraction") << 5.0 << "-" << 3.0 << 2.0;
    QTest::newRow("Multiplication") << 5.0 << "*" << 3.0 << 15.0;
    QTest::newRow("Division") << 6.0 << "/" << 2.0 << 3.0;
    QTest::newRow("Power") << 2.0 << "^" << 3.0 << 8.0;
    QTest::newRow("Square Root") << 16.0 << "sqrt" << 0.0 << 4.0;
    QTest::newRow("Logarithm (base 10)") << 100.0 << "log" << 0.0 << 2.0;
    QTest::newRow("Natural Logarithm") << 2.0 << "ln" << 0.0 << log(2.0);
}
void Test_CalculatorUtils::testPerformOperation()
{
    QFETCH(double, operand1);
    QFETCH(QString, operation);
    QFETCH(double, operand2);
    QFETCH(double, expectedResult);

    double result = CalculatorUtils::performOperation(operand1, operation, operand2);

    QCOMPARE(result, expectedResult);
}




QTEST_APPLESS_MAIN(Test_CalculatorUtils)
#include "main.moc"

