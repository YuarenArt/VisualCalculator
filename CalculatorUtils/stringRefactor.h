#pragma once
#include <QRegularExpression>
#include <QString>

namespace CalculatorUtils {

    class StringRefactor
    {
    public:
        StringRefactor(QString text);
        void updateStandartText();
        QString getFormattedText();

    private:

        QString formattedText;

        void addSpacesBetweenNumbersAndOperators();
        void addSpacesAroundBrackets();
        void addMultiplicationOperator();
        void removeExtraSpacesBetweenNumbers();
        void removeSpaceBetweenOperatorsAndNumbers();
        void formatNumbersWithOperators();
        void removeLeadingZerosAndNormalizeDecimals();
        void removeExtraDecimals();
        void replaceCommasWithPeriods();
        void handleMinusSigns();
        void removeExtraSpaces();
    };
}