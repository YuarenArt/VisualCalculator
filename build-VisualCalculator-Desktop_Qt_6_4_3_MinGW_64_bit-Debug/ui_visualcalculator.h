/********************************************************************************
** Form generated from reading UI file 'visualcalculator.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISUALCALCULATOR_H
#define UI_VISUALCALCULATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VisualCalculator
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *VisualCalculator)
    {
        if (VisualCalculator->objectName().isEmpty())
            VisualCalculator->setObjectName("VisualCalculator");
        VisualCalculator->resize(800, 600);
        centralwidget = new QWidget(VisualCalculator);
        centralwidget->setObjectName("centralwidget");
        VisualCalculator->setCentralWidget(centralwidget);
        menubar = new QMenuBar(VisualCalculator);
        menubar->setObjectName("menubar");
        VisualCalculator->setMenuBar(menubar);
        statusbar = new QStatusBar(VisualCalculator);
        statusbar->setObjectName("statusbar");
        VisualCalculator->setStatusBar(statusbar);

        retranslateUi(VisualCalculator);

        QMetaObject::connectSlotsByName(VisualCalculator);
    } // setupUi

    void retranslateUi(QMainWindow *VisualCalculator)
    {
        VisualCalculator->setWindowTitle(QCoreApplication::translate("VisualCalculator", "VisualCalculator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VisualCalculator: public Ui_VisualCalculator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISUALCALCULATOR_H
