#include "integralcalculator.h"
#include "../CalculatorUtils/CalculatorUtils.h"

#include <QMenu>
#include <QClipboard>

IntegralCalculator::IntegralCalculator(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::IntegralCalculator())
{
	ui->setupUi(this);

    // Connect various UI elements to their respective slots for user interaction
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &IntegralCalculator::updateDisplayText);
    connect(ui->equalBtn, &QPushButton::clicked, this, &IntegralCalculator::calculateResult);
    connect(ui->copyBtn, &QPushButton::clicked, this, &IntegralCalculator::copyExpressionToClipboard);
    connect(ui->clearBtn, &QPushButton::clicked, this, &IntegralCalculator::clearExpression);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &IntegralCalculator::handleEnterPressed);
    connect(ui->historyList, &QListWidget::itemDoubleClicked, this, &IntegralCalculator::historyListDoubleClicked);
}

IntegralCalculator::~IntegralCalculator()
{
	delete ui;
}

void IntegralCalculator::updateHistoryList(const QString& text)
{
	ui->historyList->addItem(text);
}

void IntegralCalculator::copyExpressionToClipboard()
{
	QString expression = ui->showResult->text();
	QApplication::clipboard()->setText(expression);
}

void IntegralCalculator::clearExpression()
{
	ui->lineEdit->clear();
	calculateResult();
}

void IntegralCalculator::handleEnterPressed()
{
	calculateResult();
}

void IntegralCalculator::historyListDoubleClicked()
{
	QListWidgetItem* item = ui->historyList->currentItem();
	ui->lineEdit->setText(item->text());
}

void IntegralCalculator::calculateResult()
{
	double upperLimit = ui->lineEditChoiceUpperLimit->text().toDouble();
    double lowerLimit = ui->lineEditChoiceLowerLimit->text().toDouble();

    QString function = ui->textShow->toPlainText();
    QString variable = ui->lineEditChoiceVariable->text();

    double result = CalculatorUtils::calculateIntegralSimpsonsMethod(function, variable, upperLimit, lowerLimit);
    ui->showResult->setText(QString::number(result, 'f'));
    updateHistoryList(ui->lineEdit->text());
}

void IntegralCalculator::updateDisplayText(const QString& text) 
{
	QString variable = ui->lineEditChoiceVariable->text();

	QString newText = CalculatorUtils::updateIntegralText(text, variable);
	ui->textShow->setText(newText);
}