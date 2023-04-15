#ifndef UDP_PARSER
#define UDP_PARSER

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

#include"udp_lexer.h"
#include"frac.h"

// struct Num
// {
//     int num;
//     int denom;
// };

using namespace std;

class Parser
{
    private:
        Lexer lexer;

    public:
        Parser(Lexer lexer) : lexer(lexer) {}

        Frac parseExpr();

};

#endif