#include "frac.h"

Frac Frac::reduce(void)
{
    int div = __gcd(this->num, this->denom);
    this->num /= div;
    this->denom /= div;
    return Frac{this->num,this->denom};
}

bool Frac::is_frac(void)
{
    return (denom != 1 ? true: false);
}

Frac Frac::minus(void)
{
    return Frac{-num,denom};
}

Frac Frac::operator+(Frac other)
{
    Frac result{0,0};
    result.num = num * other.denom + other.num * denom;
    result.denom = denom * other.denom;
    return result.reduce();
}

Frac Frac::operator-(Frac other)
{
    Frac result{0, 0};
    result.num = num * other.denom - other.num * denom;
    result.denom = denom * other.denom;
    return result.reduce();
}

Frac Frac::operator*(Frac other)
{
    Frac result{0, 0};
    result.num = num * other.num;
    result.denom = denom * other.denom;
    return result.reduce();
}

Frac Frac::operator/(Frac other)
{
    Frac result{0, 0};
    result.num = num * other.denom;
    result.denom = denom * other.num;
    return result.reduce();
}