#pragma once

#include <QRegularExpression>
#include <QString>

namespace CalculatorUtils {

    class StringRefactor
    {
    public:
        StringRefactor(QString text);

        void updateStandardText();
        void updateIntegralText(const QString& variable);
        QString getFormattedText();

        

    private:
        QString formattedText;
        QRegularExpression regex;   // ќдин экземпл€р дл€ многократного использовани€

        void addSpacesBetweenNumbersAndOperators();
        void addSpacesAroundBrackets();
        void addMultiplicationOperator();
        void removeExtraSpacesBetweenNumbers();
        void removeSpaceBetweenOperatorsAndNumbers();
        void replaceCommasWithPeriods();
        void removeExtraSpaces();
        void addSpacesAfterFuntionAndOperators();

        void ignoreVariableInFormatting(const QString variable);
    };
}
