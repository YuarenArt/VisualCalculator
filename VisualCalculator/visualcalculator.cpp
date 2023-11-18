#include "visualcalculator.h"
#include "ui_visualcalculator.h"

VisualCalculator::VisualCalculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VisualCalculator)
{
    ui->setupUi(this);
}

VisualCalculator::~VisualCalculator()
{
    delete ui;
}

