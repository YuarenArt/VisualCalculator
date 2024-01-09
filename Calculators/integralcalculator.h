#pragma once

#include <QWidget>
#include "ui_integralcalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class IntegralCalculator; };
QT_END_NAMESPACE

class IntegralCalculator : public QWidget
{
	Q_OBJECT

public:
	IntegralCalculator(QWidget *parent = nullptr);
	~IntegralCalculator();

    void updateHistoryList(const QString& text);

public slots:

    void updateDisplayText(const QString& text);
    void copyExpressionToClipboard();
    void clearExpression();
    void handleEnterPressed();
    void historyListDoubleClicked();
    void calculateResult();

private:
	Ui::IntegralCalculator *ui;
};
