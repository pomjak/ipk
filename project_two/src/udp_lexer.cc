#include "udp_lexer.h"
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