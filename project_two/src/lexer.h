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
#ifndef UDP_LEXER
#define UDP_LEXER

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

enum TokenType
{
    LPAREN,
    RPAREN,
    OPERATOR,
    NUMBER,
    SP,
    END
};

struct Token
{
    TokenType type;
    string value;
};

class Lexer
{
    private:
        string input;
        int pos;

    public:
        Lexer(string input) {
            Lexer::input = input;
            Lexer::pos = 0;
        }

        Token getNextToken();

};

#endif