#include <sstream>
#include <string>
#include "read.hpp"

bool read_t(bool*, const std::string& s) {
    bool b;
    std::istringstream(s) >> std::boolalpha >> b;
    return b;
}

char read_t(char*, const std::string& s) {
    return s[0];
}

double read_t(double*, const std::string& s) {
    return std::stod(s);
}

float read_t(float*, const std::string& s) {
    return std::stof(s);
}

int read_t(int*, const std::string& s) {
    return std::stoi(s);
}

long long read_t(long long*, const std::string& s) {
    return std::stoll(s);
}

long read_t(long*, const std::string& s) {
    return std::stol(s);
}

unsigned long read_t(unsigned long*, const std::string& s) {
    return std::stoul(s);
}

unsigned long long read_t(unsigned long long*, const std::string& s) {
    return std::stoull(s);
}
