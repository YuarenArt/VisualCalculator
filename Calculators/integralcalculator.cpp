#include "integralcalculator.h"
#include "../CalculatorUtils/CalculatorUtils.h"

#include <QMenu>
#include <QClipboard>

Integralcalculator::Integralcalculator(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Integralcalculator())
{
	ui->setupUi(this);

    // Connect various UI elements to their respective slots for user interaction
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &Integralcalculator::updateDisplayText);
    connect(ui->equalBtn, &QPushButton::clicked, this, &Integralcalculator::calculateResult);
    connect(ui->copyBtn, &QPushButton::clicked, this, &Integralcalculator::copyExpressionToClipboard);
    connect(ui->clearBtn, &QPushButton::clicked, this, &Integralcalculator::clearExpression);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &Integralcalculator::handleEnterPressed);
    connect(ui->historyList, &QListWidget::itemDoubleClicked, this, &Integralcalculator::historyListDoubleClicked);
}

Integralcalculator::~Integralcalculator()
{
	delete ui;
}

void Integralcalculator::updateHistoryList(const QString& text)
{
	ui->historyList->addItem(text);
}

void Integralcalculator::copyExpressionToClipboard()
{
	QString expression = ui->showResult->text();
	QApplication::clipboard()->setText(expression);
}

void Integralcalculator::clearExpression()
{
	ui->lineEdit->clear();
	calculateResult();
}

void Integralcalculator::handleEnterPressed()
{
	calculateResult();
}

void Integralcalculator::historyListDoubleClicked()
{
	QListWidgetItem* item = ui->historyList->currentItem();
	ui->lineEdit->setText(item->text());
}

void Integralcalculator::calculateResult()
{
	double upperLimit = ui->lineEditChoiceUpperLimit->text().toDouble();
    double lowerLimit = ui->lineEditChoiceLowerLimit->text().toDouble();

    QString function = ui->textShow->toPlainText();
    QString variable = ui->lineEditChoiceVariable->text();

    double result = CalculatorUtils::calculateIntegralSimpsonsMethod(function, variable, upperLimit, lowerLimit);
    ui->showResult->setText(QString::number(result, 'f'));
    updateHistoryList(ui->lineEdit->text());
}

void Integralcalculator::updateDisplayText(const QString& text) 
{
	QString newText = CalculatorUtils::updateText(text);
	ui->textShow->setText(newText);
}