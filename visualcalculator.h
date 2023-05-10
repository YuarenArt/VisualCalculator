#pragma once

#include <QtWidgets/QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QRegularExpression>
#include <QMessageBox>

class VisualCalculator : public QMainWindow
{
    Q_OBJECT

public:
    VisualCalculator(QWidget *parent = nullptr);

    bool isValidInput(const QString& input);

private:
    QTextEdit* textShow;
    QLineEdit* lineEdit;
    
public slots:

    void updateTextShow(const QString& text);

};
