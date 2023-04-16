#include "parser.h"
#include "lexer.h"
#include "frac.h"
#include "ipkcpd.h"

Frac Parser::parseExpr()
{
    Token token = lexer.getNextToken();

    if (token.type == LPAREN) 
    {
        stack<Frac> stack;
        string op = lexer.getNextToken().value;

        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: missing space OP<SPACE>EXP");

        stack.push(parseExpr());


        if (lexer.getNextToken().type != SP)
            throw runtime_error("PARSER: missing space EXP<SPACE>EXP");

        stack.push(parseExpr());


        while((token = lexer.getNextToken()).type != RPAREN)
        {
            if (token.type == SP)
            {
                stack.push(parseExpr());
            }
            else
                throw runtime_error("PARSER: missing right parenthesis");
        }

        if (op == "+")
        {
            Frac result = stack.top();
            stack.pop();
            while (!stack.empty())
            {
                result = result + stack.top();
                stack.pop();
            }
            return result;
        }

        else if (op == "-")
        {
            reverse_stack_order(&stack);
            Frac result = stack.top();
            stack.pop();
            while (!stack.empty())
            {
                result = result - stack.top();

                stack.pop();
            }
            return result;
        }

        else if (op == "*")
        {
            Frac result = stack.top();
            stack.pop();
            while (!stack.empty())
            {
                result = result * stack.top();

                stack.pop();
            }
            return result;
        }

        else if (op == "/")
        {
            {
                reverse_stack_order(&stack);
                Frac result = stack.top();
                stack.pop();
                while (!stack.empty())
                {
                    if(!stack.top().get_num())
                        throw runtime_error("PARSER: division by 0");
                    result = result / stack.top();

                    stack.pop();
                }
                return result;
            }
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

    throw runtime_error("PARSER: cant parse that");
}