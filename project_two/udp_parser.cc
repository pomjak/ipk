#include "udp_parser.h"
#include "udp_lexer.h"

int Parser::parseExpr()
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN) //(
    {
        string op = lexer.getNextToken().value;// op
        lexer.getNextToken();// SP
        int left = parseExpr();// num
        lexer.getNextToken();// SP
        int right = parseExpr();//num
        lexer.getNextToken();//)
        
        if (op == "+")
            return left + right;
        else if (op == "-")
            return left - right;
    
        else if (op == "*")
            return left * right;
        
        else if (op == "/")
            return left / right;
        
    }
    else if (token.type == NUMBER)
        return stoi(token.value);
    
    throw runtime_error("PARSER: parseExpr: Invalid expression");
}

int Parser::parseQuery(void)
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN)
    {
        string op = lexer.getNextToken().value;// op
        lexer.getNextToken();       // SP
        int left = parseExpr();     // num
        lexer.getNextToken();       // SP
        int right = parseExpr();    //num
        lexer.getNextToken();       //)

        if (op == "+")
            return left + right;
    
        else if (op == "-")
            return left - right;
        
        else if (op == "*")
            return left * right;
        
        else if (op == "/")
            return left / right; 
    }

    throw runtime_error("PARSER: parseQuery: Invalid query");
}