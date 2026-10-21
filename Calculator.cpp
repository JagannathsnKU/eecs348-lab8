#include "Calculator.h"
#include <sstream>
#include <cctype>

std::vector<std::string> Calculator::split(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);
    return tokens;
}

bool Calculator::isValidDouble(const std::string& s) {
    size_t i = 0, n = s.size();
    if (i < n && (s[i] == '+' || s[i] == '-')) i++;

    size_t intDigits = 0;
    while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) { i++; intDigits++; }
    if (intDigits == 0) return false;

    if (i == n) return true;          // integer form
    if (s[i] != '.') return false;
    i++;

    size_t fracDigits = 0;
    while (i < n && std::isdigit(static_cast<unsigned char>(s[i]))) { i++; fracDigits++; }
    if (fracDigits == 0) return false;

    return i == n;                    // nothing may follow
}

Calculator::Num Calculator::parse(const std::string& s) {
    Num r;
    r.neg = false;
    size_t i = 0;
    if (s[i] == '+' || s[i] == '-') { r.neg = (s[i] == '-'); i++; }

    size_t dot = s.find('.', i);
    if (dot == std::string::npos) {
        r.intPart = s.substr(i);
        r.frac = "";
    } else {
        r.intPart = s.substr(i, dot - i);
        r.frac = s.substr(dot + 1);
    }

    size_t nz = r.intPart.find_first_not_of('0');
    r.intPart = (nz == std::string::npos) ? "0" : r.intPart.substr(nz);

    size_t last = r.frac.find_last_not_of('0');
    r.frac = (last == std::string::npos) ? "" : r.frac.substr(0, last + 1);
    return r;
}

std::string Calculator::addDigits(const std::string& a, const std::string& b) {
    std::string res;
    int carry = 0;
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        res.push_back(static_cast<char>('0' + sum % 10));
        carry = sum / 10;
    }
    if (carry) res.push_back('1');
    return std::string(res.rbegin(), res.rend());
}

std::string Calculator::subDigits(const std::string& a, const std::string& b) {
    std::string res;
    int borrow = 0;
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
        int d = (a[i] - '0') - (b[i] - '0') - borrow;
        if (d < 0) { d += 10; borrow = 1; } else borrow = 0;
        res.push_back(static_cast<char>('0' + d));
    }
    return std::string(res.rbegin(), res.rend());
}

int Calculator::compareDigits(const std::string& a, const std::string& b) {
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

std::string Calculator::add(const std::string& sa, const std::string& sb) {
    Num a = parse(sa), b = parse(sb);

    // Align decimal points by padding with zeros.
    size_t L = std::max(a.intPart.size(), b.intPart.size());
    size_t F = std::max(a.frac.size(), b.frac.size());
    std::string da = std::string(L - a.intPart.size(), '0') + a.intPart + a.frac + std::string(F - a.frac.size(), '0');
    std::string db = std::string(L - b.intPart.size(), '0') + b.intPart + b.frac + std::string(F - b.frac.size(), '0');

    std::string digits;
    bool neg;
    if (a.neg == b.neg) {
        digits = addDigits(da, db);
        neg = a.neg;
    } else {
        int cmp = compareDigits(da, db);
        if (cmp == 0) {
            digits = std::string(da.size(), '0');
            neg = false;
        } else if (cmp > 0) {
            digits = subDigits(da, db);
            neg = a.neg;
        } else {
            digits = subDigits(db, da);
            neg = b.neg;
        }
    }

    // Split back into integer / fractional parts (carry may add a digit).
    std::string intPart = digits.substr(0, digits.size() - F);
    std::string frac = digits.substr(digits.size() - F);

    size_t nz = intPart.find_first_not_of('0');
    intPart = (nz == std::string::npos) ? "0" : intPart.substr(nz);
    size_t last = frac.find_last_not_of('0');
    frac = (last == std::string::npos) ? "" : frac.substr(0, last + 1);

    std::string out = intPart;
    if (!frac.empty()) out += "." + frac;
    if (neg && out != "0") out = "-" + out;
    return out;
}
