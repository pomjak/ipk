#include "udp_parser.h"
#include "udp_lexer.h"

Num Parser::parseExpr()
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN) //(
    {
        string op = lexer.getNextToken().value;// op
        lexer.getNextToken();// SP
        Num left = parseExpr();// num
        lexer.getNextToken();// SP
        Num right = parseExpr();//num
        lexer.getNextToken();//)

        if (op == "+")
            return Num{(left.num + right.num),0};

        else if (op == "-")
            return Num{(left.num - right.num), 0};

        else if (op == "*")
            return Num{(left.num * right.num), 0};

        else if (op == "/")
        {
            if (right.num == 0)
                throw runtime_error("PARSER: parseExpr: Division by zero");
            else
            {
                int div = __gcd(left.num,right.num);
                left.num /= div;
                right.num /= div;
                if(right.num == 1)
                    return Num{left.num , 0};
                else 
                    return Num{left.num, right.num};
            }
        }
    }
    else if (token.type == NUMBER)
    {
        return Num{stoi(token.value),0};
    }
    else if (token.type == OPERATOR && token.value == "-")
    {
        int num = parseExpr().num;
        return Num{-num,0}; 
    }
    
    throw runtime_error("PARSER: parseExpr: Invalid expression");
}

Num Parser::parseQuery(void)
{
    Token token = lexer.getNextToken();
    if (token.type == LPAREN)
    {
        string op = lexer.getNextToken().value;// op
        lexer.getNextToken();       // SP
        Num left = parseExpr();     // num
        lexer.getNextToken();       // SP
        Num right = parseExpr();    //num
        lexer.getNextToken();       //)

        if (op == "+")
            return Num{(left.num + right.num), 0};

        else if (op == "-")
            return Num{(left.num - right.num), 0};

        else if (op == "*")
            return Num{(left.num * right.num), 0};

        else if (op == "/")
        {
            if (right.num == 0)
                throw runtime_error("PARSER: parseExpr: Division by zero");
            else
            {
                int div = __gcd(left.num, right.num);
                left.num /= div;
                right.num /= div;
                if (right.num == 1)
                    return Num{left.num, 0};
                else
                    return Num{left.num, right.num};
            }
        }
    }
    else if (token.type == NUMBER)
    {
        cout << "token type is number" << endl;
        return Num{stoi(token.value),0};
    }

    throw runtime_error("PARSER: parseQuery: Invalid query");
}