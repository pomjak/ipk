#ifndef UDP_PARSER
#define UDP_PARSER

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

#include"udp_lexer.h"

using namespace std;

class Parser
{
    private:
        Lexer lexer;

    public:
        Parser(Lexer lexer) : lexer(lexer) {}

        int parseExpr(void);

        int parseQuery(void);
};

#endif