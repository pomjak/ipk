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