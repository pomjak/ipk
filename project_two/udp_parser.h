#ifndef UDP_PARSER
#define UDP_PARSER

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

#include"udp_lexer.h"

struct Num
{
    int num;
    int denom;
};

using namespace std;

class Parser
{
    private:
        Lexer lexer;

    public:
        Parser(Lexer lexer) : lexer(lexer) {}

        Num parseExpr(void);

        Num parseQuery(void);
};

#endif