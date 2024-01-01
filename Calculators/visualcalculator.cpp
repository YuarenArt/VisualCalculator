#include "visualcalculator.h"
#include <QMenu>
#include <QComboBox>

const int standartID = 0;
const int integralID = 1;

VisualCalculator::VisualCalculator(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::VisualCalculator()), standartCalculator(new StandartCalculator), integralCalculator(new Integralcalculator)
{
	ui->setupUi(this);

	ui->stackedWidget->insertWidget(standartID, standartCalculator);
	ui->stackedWidget->insertWidget(integralID, integralCalculator);

	ui->stackedWidget->setCurrentIndex(0);

	connect(ui->choiceCalculatorMenu, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &VisualCalculator::onChoiceCalculatorMenuChanged);
}

VisualCalculator::~VisualCalculator()
{
	delete ui;
}
