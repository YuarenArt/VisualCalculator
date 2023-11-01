#pragma once

#include <QObject>

class CalculatorInterface  : public QObject
{
	Q_OBJECT

public:
	CalculatorInterface(QObject *parent);
	virtual ~CalculatorInterface() = default;


public slots:

	virtual void calculate() = 0;
	virtual void updateDisplayText(const QString& text) = 0;
	virtual void copyExpressionToClipboard() = 0;
	virtual void clearExpression() = 0;
	virtual void handleEnterPressed() = 0;
	virtual void historyListDoubleClicked() = 0;
};

Q_DECLARE_INTERFACE(CalculatorInterface)