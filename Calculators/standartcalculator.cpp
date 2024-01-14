#include "standartcalculator.h"
#include "../CalculatorUtils/CalculatorUtils.cpp"

#include "QMenu"
#include <QClipboard>

StandartCalculator::StandartCalculator(QWidget* parent)
    : QWidget(parent), ui(new Ui::StandartCalculator)
{
    ui->setupUi(this);
    

    // Connect various UI elements to their respective slots for user interaction
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &StandartCalculator::updateDisplayText);
    connect(ui->equalBtn, &QPushButton::clicked, this, &StandartCalculator::calculateResult);
    connect(ui->copyBtn, &QPushButton::clicked, this, &StandartCalculator::copyExpressionToClipboard);
    connect(ui->clearBtn, &QPushButton::clicked, this, &StandartCalculator::clearExpression);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &StandartCalculator::handleEnterPressed);
    connect(ui->historyList, &QListWidget::itemDoubleClicked, this, &StandartCalculator::historyListDoubleClicked);
}

StandartCalculator::~StandartCalculator()
{
	delete ui;
}

void StandartCalculator::updateHistoryList(const QString& text)
{
    ui->historyList->addItem(text);
}

void StandartCalculator::calculateResult()
{
    QString expression = ui->textShow->toPlainText();

    if (expression.isEmpty()) {
        ui->showResult->setText(" ");
        return;
    }

    double result = CalculatorUtils::calculateExpressionWithRPN(expression);
    updateHistoryList(expression);
    ui->showResult->setText(QString::number(result));
}

void StandartCalculator::updateDisplayText(const QString& text)
{
    QString newText = CalculatorUtils::updateStandartText(text);
    ui->textShow->setText(newText);
}

void StandartCalculator::copyExpressionToClipboard()
{
    QString expression = ui->showResult->text();
    QApplication::clipboard()->setText(expression);
}

void StandartCalculator::clearExpression()
{
    ui->lineEdit->clear();
    calculateResult();
}

void StandartCalculator::handleEnterPressed()
{
    calculateResult();
}

void StandartCalculator::historyListDoubleClicked()
{
    QListWidgetItem* item = ui->historyList->currentItem();
    ui->lineEdit->setText(item->text());
}


