#include <iostream>
#include <string>
#include "IO.func.hpp"
#include "show.hpp"
#include "read.hpp"

#ifndef IO_HPP
#define IO_HPP

template<typename Ta, typename Tb>
inline IO<Tb> ignoreA(IO<Ta> a, IO<Tb> b) { return IO(a, b);}

// Write a character to the standard output device
inline IO<void*> putChar(char c) {
    return IO(asF<void*>([c] {std::cout << c; return (void*)nullptr; }));
};

// Write a string to the standard output device
inline IO<void*> putStr(std::string s) {
    return IO(asF<void*>([s] () {std::cout << s; return (void*)nullptr; }));
};

// The same as `putStr`, but adds a newline character
inline IO<void*> putStrLn(std::string s) {
    return IO(asF<void*>([s] () {std::cout << s << std::endl; return (void*)nullptr; }));
};

inline IO<char> getChar() {
    return IO(asF<char>([] () {char c; std::cin >> c; return c;}));
}

inline IO<std::string> getLine() {
    return IO(asF<std::string>([] () {std::string s; std::getline(std::cin, s); return s;}));
}

// The `print` function outputs a value of any printable type to the standard output device.
// Printable types are those that are instances of class `Show`; `print` converts values to strings 
// for ouput using the `show` operation and adds a newline.
template<typename T>
requires (Show<T>) inline IO<void*> print(T x) {
    return IO(asF<void*>([x] () {std::cout << show(x) << std::endl; return (void*)nullptr; }));
}

template<typename T>
inline IO<void*> print(const T& x) {
    return IO(asF<void*>([x] () {std::cout << show(x) << std::endl; return (void*)nullptr; }));
}

// Read input from std::cin
template<typename T>
inline IO<T> readln() {
    return fmap(asF<T,std::string>(read<T>), getLine());
};

#endif
