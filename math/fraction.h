#pragma once

#include <algorithm>
#include <iostream>

/*
 * I have definitely written a class like this before. Not necessarily in cpp
 * but definitely in ocaml. For now I want it to be a header file because I am
 * not ready to do a build system for csml.
 * nickbhorton 08/23/2024
 */

namespace csml
{
namespace math
{

inline int gcd(int a, int b)
{
    int result = std::min(a, b);
    while (result > 0) {
        if (a % result == 0 && b % result == 0) {
            break;
        }
        result--;
    }
    return result;
}

enum class Operator { Plus, Minus };
struct Fraction {
private:
    int numerator;
    int denominator;

public:
    Fraction(int numerator, int denominator)
    {
        if (numerator != 0 && denominator < 0) {
            this->numerator = -numerator;
            this->denominator = -denominator;
        } else if (denominator == 0) {
            this->numerator = 0;
            this->denominator = 0;
            return;
        } else if (numerator == 0) {
            this->numerator = 0;
            this->denominator = 1;
            return;
        } else {
            this->numerator = numerator;
            this->denominator = denominator;
        }
        int factor{
            gcd(this->numerator < 0 ? -this->numerator : this->numerator,
                this->denominator)
        };
        if (factor > 0) {
            this->numerator /= factor;
            this->denominator /= factor;
        } else {
            std::cerr << "the gcd of " << this->numerator << " and "
                      << this->denominator << " was " << factor << "\n";
            std::exit(1);
        }
    }
    friend std::ostream& operator<<(std::ostream& os, Fraction const& f);
    friend Fraction
    apply_op(Fraction const& f1, Fraction const& f2, const Operator& op);
};

inline std::ostream& operator<<(std::ostream& os, Fraction const& f)
{
    os << f.numerator << "/" << f.denominator;
    return os;
}

inline Fraction
apply_op(Fraction const& f1, Fraction const& f2, Operator const& op)
{
    switch (op) {
    case Operator::Plus:
        return Fraction(
            f1.numerator * f2.denominator + f2.numerator * f1.denominator,
            f1.denominator * f2.denominator
        );
    case Operator::Minus:
        return Fraction(
            f1.numerator * f2.denominator - f2.numerator * f1.denominator,
            f1.denominator * f2.denominator
        );
    }
    return Fraction(0, 0);
}

inline Fraction operator+(Fraction const& f1, Fraction const& f2)
{
    return apply_op(f1, f2, Operator::Plus);
}

inline Fraction operator-(Fraction const& f1, Fraction const& f2)
{
    return apply_op(f1, f2, Operator::Minus);
}

} // namespace math
} // namespace csml
