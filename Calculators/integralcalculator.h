#pragma once

#include <QWidget>
#include "ui_integralcalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Integralcalculator; };
QT_END_NAMESPACE

class Integralcalculator : public QWidget
{
	Q_OBJECT

public:
	Integralcalculator(QWidget *parent = nullptr);
	~Integralcalculator();

    void updateHistoryList(const QString& text);

public slots:

    void updateDisplayText(const QString& text);
    void copyExpressionToClipboard();
    void clearExpression();
    void handleEnterPressed();
    void historyListDoubleClicked();
    void calculateResult();

private:
	Ui::Integralcalculator *ui;
};
