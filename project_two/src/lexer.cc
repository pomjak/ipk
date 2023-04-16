/*
 * IPK Project 2: IOTA: Server for Remote Calculator
 * Author: xpomsa00
 * Copyright (C) 2023 Pomsar Jakub FIT VUT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "lexer.h"
#include "ipkcpd.h"

Token Lexer::getNextToken()
{
    if (pos >= (int)input.length())
    {
        cout << "end" << endl;
        return Token{END, ""};
    }

    char currentChar = input[pos];

    if(currentChar == ' ')
    {
        pos++;
        cout << "lexer : SP" << endl;
        return Token{SP, " "};
    }

    if (currentChar == '(')
    {
        pos++;
        cout << "lexer : ("<<endl;
        return Token{LPAREN, "("};
    }

    if (currentChar == ')')
    {
        pos++;
        cout << "lexer : )" << endl;
        return Token{RPAREN, ")"};
    }

    if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')
    {
        pos++;
        cout << "lexer : opertator" << endl;
        return Token{OPERATOR, string(1, currentChar)};
    }

    if (isdigit(currentChar))
    {
        string number = "";
        while (pos < (int)input.length() && isdigit(input[pos]))
        {
            number += input[pos];
            cout << "lexer : number "<< number << endl;
            pos++;
        }
        return Token{NUMBER, number};
    }

    throw runtime_error("LEXER: getNextToken: Invalid character: " + string(1, currentChar));
}