#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Calculator.h"

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        std::cout << "Enter test file name: ";
        std::getline(std::cin, filename);
    }

    std::ifstream in(filename.c_str());
    if (!in) {
        std::cerr << "ERROR: cannot open file \"" << filename << "\"" << std::endl;
        return 1;
    }

    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        lineNo++;
        std::vector<std::string> t = Calculator::split(line);

        if (t.size() != 2) {
            std::cout << "line " << lineNo << ": ERROR: expected exactly two numbers" << std::endl;
            continue;
        }

        bool v0 = Calculator::isValidDouble(t[0]);
        bool v1 = Calculator::isValidDouble(t[1]);
        if (!v0 || !v1) {
            std::cout << "line " << lineNo << ": ERROR: invalid number";
            if (!v0) std::cout << " \"" << t[0] << "\"";
            if (!v1) std::cout << " \"" << t[1] << "\"";
            std::cout << std::endl;
            continue;
        }

        std::cout << "line " << lineNo << ": " << t[0] << " + " << t[1]
                  << " = " << Calculator::add(t[0], t[1]) << std::endl;
    }
    return 0;
}
