#pragma once

#include <QMainWindow>
#include "ui_visualcalculator.h"

#include "standartcalculator.h"
#include "integralcalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VisualCalculator; };
QT_END_NAMESPACE

class VisualCalculator : public QMainWindow
{
	Q_OBJECT

public:
	VisualCalculator(QWidget *parent = nullptr);
	~VisualCalculator();

private:
	Ui::VisualCalculator *ui;
	IntegralCalculator* integralCalculator;
	StandartCalculator* standartCalculator;

private slots:
	void onChoiceCalculatorMenuChanged(int index);
};
