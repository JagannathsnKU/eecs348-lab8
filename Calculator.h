#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>

// String-based arithmetic. No string is ever converted to a numeric type.
class Calculator {
public:
    // Returns true if s matches (+|-)?D+(.D+)?
    static bool isValidDouble(const std::string& s);

    // Adds two VALID double strings; returns normalized result string
    // (no leading zeros, no trailing fractional zeros, no "-0").
    static std::string add(const std::string& a, const std::string& b);

    // Splits a line into whitespace-separated tokens.
    static std::vector<std::string> split(const std::string& line);

private:
    struct Num {
        bool neg;
        std::string intPart;  // digits, no leading zeros ("0" if zero)
        std::string frac;     // digits, no trailing zeros ("" if none)
    };

    static Num parse(const std::string& s);
    static std::string addDigits(const std::string& a, const std::string& b);
    static std::string subDigits(const std::string& a, const std::string& b); // a >= b
    static int compareDigits(const std::string& a, const std::string& b);     // equal length
};

#endif
