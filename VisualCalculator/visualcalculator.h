#ifndef VISUALCALCULATOR_H
#define VISUALCALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class VisualCalculator; }
QT_END_NAMESPACE

class VisualCalculator : public QMainWindow
{
    Q_OBJECT

public:
    VisualCalculator(QWidget *parent = nullptr);
    ~VisualCalculator();

private:
    Ui::VisualCalculator *ui;
};
#endif // VISUALCALCULATOR_H
