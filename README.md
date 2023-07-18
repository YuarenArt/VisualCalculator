# Visual-calculator
This is a simple calculator application built using Qt framework. It allows users to perform basic arithmetic calculations and converts expressions to reverse Polish notation (RPN).

You need to connect a third-party project CalculatorUtils (https://github.com/YuarenArt/CalculatorUtils), 
which defines functions for calculations and transformations that do not depend on the Visualcalculator class


Features

Arithmetic calculations: 
The calculator supports addition, subtraction, multiplication, and division operations.

Input validation: The input is validated to ensure that only valid characters and operators are accepted.

Conversion to RPN: The application includes a method to convert an arithmetic expression to reverse Polish notation.


Usage

Enter an arithmetic expression in the input field.

Press the "=" button to calculate the result.

The result will be displayed in the output field.

The input expression, with spaces added between numbers and operators, will be shown below the output field.

Example

Suppose you want to calculate the result of the expression: 5 + (2 * 3) / 4.

Enter the expression: 5+(2*3)/4.

Press the "=" button.

The result will be displayed as 6.5 in the output field.

The input expression will be shown as 5 + (2 * 3) / 4 below the output field.


Note

Division by zero is handled, and a warning message is displayed when attempting to divide by zero.

The application uses the following operator precedence: * and / have higher precedence than + and -.
