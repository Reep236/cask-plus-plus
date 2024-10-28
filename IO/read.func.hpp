#include <functional>
#include <list>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

#ifndef READ_FUNC_HPP
#define READ_FUNC_HPP

// Internal: Using a phantom for type resolution, read input from a string. 
template<typename T>
T read_t(T* phantom, const std::string& x) = delete;

template<template<typename...> class C, typename T>
C<T> read_c(std::function<T(T*, const std::string&)> readF, C<T>* phantom, std::string x) = delete;

template<template<typename, typename> class C, typename T1, typename T2>
C<T1, T2> read_c2(std::function<T1(T1*, const std::string&)> readF1, std::function<T2(T2*, std::string)> readF2, 
                  C<T1, T2>* phantom, const std::string& x) = delete;

bool read_t(bool* _, const std::string& s);

char read_t(char* _, const std::string& s);

double read_t(double* _, const std::string& s);

float read_t(float* _, const std::string& s);

int read_t(int* _, const std::string& s);

long long read_t(long long* _, const std::string& s);

long read_t(long* _, const std::string& s);

unsigned long read_t(unsigned long* _, const std::string& s);

unsigned long long read_t(unsigned long long* _, const std::string& s);

template<typename T> 
std::list<T> read_c(std::function<T(T*, const std::string&)> readF, std::list<T>*, const std::string& x) {
    std::string s = x; // Unfortunate copy 
    std::replace_if(s.begin(), s.end(), [] (char c) -> bool {return c == ',' || c == '[' || c == ']';}, ' ');
    std::istringstream xStream(s);
    std::list<T> ys;
    T* phantom = nullptr;
    while (xStream >> s) {
        if (!xStream.good())
            throw std::invalid_argument("read (std::list): no parse");
        ys.emplace_back(readF(phantom, s));
    };
    return ys;
}
#endif
