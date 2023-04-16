#include "parser.h"
#include "lexer.h"
#include "frac.h"

Frac Parser::parseExpr()
{
    Token token = lexer.getNextToken();

    if (token.type == LPAREN) 
    {
        string op = lexer.getNextToken().value;

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: missing space OP<SPACE>EXP");

        Frac left = parseExpr();

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: missing space EXP<SPACE>EXP");

        Frac right = parseExpr();

        while(lexer.getNextToken().type != RPAREN)
        {
            // if (lexer.getNextToken().type == SP)
            // {

            // }
            // else
                throw runtime_error("PARSER: missing right parenthesis");
        }

        if (op == "+")
            return left + right;

        else if (op == "-")
            return left - right;

        else if (op == "*")
            return left * right;

        else if (op == "/")
        {   
            if(right.get_num() == 0)
                throw runtime_error("PARSER: division by 0");
            return left / right;
        }
        else
            throw runtime_error("PARSER: unrecognized operator");
    }
    else if (token.type == NUMBER)
    {
        return Frac{stoi(token.value),1};
    }

    else if (token.type == OPERATOR && token.value == "-")
    {
        throw runtime_error("PARSER: not supported operator -");
    }

    throw runtime_error("PARSER: missing left parenthesis");
}