/*
 * IPK Project 2: IOTA: Server for Remote Calculator
 * Author: xpomsa00
 * Copyright (C) 2023 Pomsar Jakub FIT VUT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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