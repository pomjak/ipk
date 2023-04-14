#ifndef CALC
#define CALC

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

class Frac
{
private:
    int num;
    int denom;
    Frac reduce(void);

public :

    Frac(int num, int denom)
    {
        this->num = num;
        this->denom = denom;
    }
    bool is_frac(void);

    int get_num(void){return num;}

    int get_denom(void){return denom;}

    Frac minus(void);

    Frac operator+(Frac y);

    Frac operator-(Frac y);
    
    Frac operator*(Frac y);
    
    Frac operator/(Frac y);
};

#endif