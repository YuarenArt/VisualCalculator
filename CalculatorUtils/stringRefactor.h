#pragma once

#include <QRegularExpression>
#include <QString>

namespace CalculatorUtils {

    class StringRefactor
    {
    public:
        StringRefactor(QString text);

        void updateStandardText();
        QString getFormattedText();

        

    private:
        QString formattedText;
        QRegularExpression regex;   // ќдин экземпл€р дл€ многократного использовани€

        void addSpacesBetweenNumbersAndOperators();
        void addSpacesAroundBrackets();
        void addMultiplicationOperator();
        void removeExtraSpacesBetweenNumbers();
        void removeSpaceBetweenOperatorsAndNumbers();
        void formatNumbersWithOperators();
        void removeLeadingZerosAndNormalizeDecimals();
        void removeExtraDecimals();
        void replaceCommasWithPeriods();
        void removeExtraSpaces();
    };
}
