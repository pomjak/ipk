#include "udp_parser.h"
#include "udp_lexer.h"
#include "frac.h"

Frac Parser::parseExpr()
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN) //(
    {
        string op = lexer.getNextToken().value;// op
        lexer.getNextToken();// SP
        Frac left = parseExpr();// num
        lexer.getNextToken();// SP
        Frac right = parseExpr();//num
        lexer.getNextToken();//)

        if (op == "+")
            return left + right;

        else if (op == "-")
            return left - right;

        else if (op == "*")
            return left * right;

        else if (op == "/")
        {
            return left / right;
        }
    }
    else if (token.type == NUMBER)
    {
        return Frac{stoi(token.value),1};
    }
    else if (token.type == OPERATOR && token.value == "-")
    {
        Frac num = parseExpr();
        return num.minus(); 
    }
    
    throw runtime_error("PARSER: parseExpr: Invalid expression");
}

Frac Parser::parseQuery(void)
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN) //(
    {
        string op = lexer.getNextToken().value; // op
        lexer.getNextToken();                   // SP
        Frac left = parseExpr();                // num
        lexer.getNextToken();                   // SP
        Frac right = parseExpr();               // num
        lexer.getNextToken();                   //)

        if (op == "+")
            return left + right;

        else if (op == "-")
            return left - right;

        else if (op == "*")
            return left * right;

        else if (op == "/")
        {
            return left / right;
        }
    }
    else if (token.type == NUMBER)
    {
        return Frac{stoi(token.value), 1};
    }
    else if (token.type == OPERATOR && token.value == "-")
    {
        Frac num = parseExpr();
        return num.minus();
    }

    throw runtime_error("PARSER: parseQuery: Invalid query");
}