#include "visualcalculator.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    VisualCalculator calculator;
    calculator.resize(350, 500);
    calculator.show();

    return a.exec();
    
}
