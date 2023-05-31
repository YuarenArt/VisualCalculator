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
#include <QStack>
#include <QtWidgets/QApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class VisualCalculator; }
QT_END_NAMESPACE


class VisualCalculator : public QMainWindow
{
    Q_OBJECT

public:
    VisualCalculator(QWidget *parent = nullptr);
    ~VisualCalculator();

    void updateHistoryList(const QString& text);

private:

    Ui::VisualCalculator* ui;
    
public slots:

    void updateDisplayText(const QString& text);
    void calculateResult();
    void copyExpressionToClipboard();
    void clearExpression();
    void handleEnterPressed();
    void historyListDoubleClicked();


};
