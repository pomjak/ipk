#include "udp_parser.h"
#include "udp_lexer.h"
#include "frac.h"

Frac Parser::parseExpr()
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN) 
    {
        string op = lexer.getNextToken().value;

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: parseExpr: missing space");

        Frac left = parseExpr();

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: parseExpr: missing space");

        Frac right = parseExpr();

        if(lexer.getNextToken().type != RPAREN)
            throw runtime_error("PARSER: parseExpr: missing right parenthesis");

        if (op == "+")
            return left + right;

        else if (op == "-")
            return left - right;

        else if (op == "*")
            return left * right;

        else if (op == "/")
        {   
            if(right.get_num() == 0)
                throw runtime_error("PARSER: parseExpr: division by 0");
            return left / right;
        }
        else
            throw runtime_error("PARSER: parseExpr: unrecognized operator");
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
        string op = lexer.getNextToken().value;

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: parseQuery: missing space");

        Frac left = parseExpr();

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: parseQuery: missing space");

        Frac right = parseExpr();

        if (lexer.getNextToken().type != RPAREN)
            throw runtime_error("PARSER: parseQuery: missing right parenthesis");

        if (op == "+")
            return left + right;

        else if (op == "-")
            return left - right;

        else if (op == "*")
            return left * right;

        else if (op == "/")
        {
            if (right.get_num() == 0)
                throw runtime_error("PARSER: parseExpr: division by 0");
            return left / right;
        }
        else
            throw runtime_error("PARSER: parseExpr: unrecognized operator");
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