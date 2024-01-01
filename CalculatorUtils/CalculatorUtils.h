#pragma once

#include <QString>
#include "stringRefactor.h"

namespace CalculatorUtils {

	int getOperatorPrecedence(const QString& op);
	bool isValidInput(const QString& input);
	bool isOperator(const QString& input);
	bool isNegativeNumber(const QString& input);
	bool isOperatorWithOneOperand(const QString& input);
	QString autoBalanceParentheses(QString& expression);
	double performOperation(double operand1, const QString& operation, double operand2 = 0);
	QStringList convertToRPN(const QString& expression);
	double calculateExpressionWithRPN(const QString& expression);
	QString updateText(const QString& text);
	double calculateExpressionWithVariable(const QString& expression, const QString& variable, double variableValue);
	double calculateIntegralSimpsonsMethod(QString& function, const QString& variable, const double& upperLimit,
		const double& lowerLimit, const qint64& n = 1000);

	class StringRefactor;

}