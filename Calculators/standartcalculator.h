#pragma once

#include <QWidget>
#include "ui_standartcalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StandartCalculator; };
QT_END_NAMESPACE

class StandartCalculator : public QWidget
{
	Q_OBJECT

public:
	StandartCalculator(QWidget *parent = nullptr);
	~StandartCalculator();

    void updateHistoryList(const QString& text);

public slots:

    void updateDisplayText(const QString& text);
    void copyExpressionToClipboard();
    void clearExpression();
    void handleEnterPressed();
    void historyListDoubleClicked();
    void calculateResult();

private:
	Ui::StandartCalculator *ui;
};
