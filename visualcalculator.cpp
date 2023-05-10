#include "visualcalculator.h"

VisualCalculator::VisualCalculator(QWidget *parent) : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    //основной вертикальный виджет куда всё размещается
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    textShow = new QTextEdit(centralWidget);
    textShow->setReadOnly(true);
    mainLayout->addWidget(textShow);

    // Установка размера шрифта
    QFont font = textShow->font();
    font.setPointSize(font.pointSize() + 6);
    textShow->setFont(font);

    QHBoxLayout* inputLayout = new QHBoxLayout;
    mainLayout->addLayout(inputLayout);

    // создания поля ввода
    lineEdit = new QLineEdit(centralWidget);
    inputLayout->addWidget(lineEdit);


    // Создание кнопки "="
    QPushButton* equalsButton = new QPushButton("=");
    inputLayout->addWidget(equalsButton);

    QLabel* rez = new QLabel;
    mainLayout->addWidget(rez);

    connect(lineEdit, &QLineEdit::textChanged, this, &VisualCalculator::updateTextShow);
}

bool VisualCalculator::isValidInput(const QString& input)
{
    QRegularExpression validRegex("^[0-9()+\\-*/. ]+$");
    QRegularExpressionMatch match = validRegex.match(input);
    return match.hasMatch();
}


void VisualCalculator::updateTextShow(const QString& text)
{
    QString formattedText = text;

    if (!isValidInput(text))
    {
        QMessageBox::information(this, "Error", "Invalid input!");
    }

    // Проверяем наличие символа '/' в строке
    QRegularExpression divisionRegex("\\/");
    QRegularExpressionMatch divisionMatch = divisionRegex.match(formattedText);
    if (divisionMatch.hasMatch())
    {
        // Форматируем деление в виде дроби
        formattedText.replace(divisionMatch.capturedStart(), divisionMatch.capturedLength(), " / ");

        // Проверяем, есть ли числа в скобках
        QRegularExpression bracketRegex("\\((\\d+)\\)");
        QRegularExpressionMatchIterator bracketIterator = bracketRegex.globalMatch(formattedText);
        while (bracketIterator.hasNext())
        {
            QRegularExpressionMatch bracketMatch = bracketIterator.next();
            QString numberInBrackets = bracketMatch.captured(1);
            formattedText.replace(bracketMatch.capturedStart(), bracketMatch.capturedLength(), numberInBrackets);
        }

        // Устанавливаем стиль для дроби
        formattedText = "<sup>" + formattedText + "</sup>";
    }

    // Обновляем текст в textShow
    textShow->setText(formattedText);
}

