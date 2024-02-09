#include "./Calculators/visualcalculator.h"
#include <QStyleFactory>
#include <QFile>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QFile styleFile("StyleSheet.css");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    qApp->setStyleSheet(styleSheet);

    VisualCalculator calculator;
    calculator.resize(350, 500);
    calculator.show();

    return a.exec();
    
}
