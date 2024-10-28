#include <functional>
#include <string>
#ifndef SHOW_FUNC_HPP
#define SHOW_FUNC_HPP

// Convert a value to a readable String | Unchecked, unsafe
template<typename T>
std::string show_t(T) = delete;

// Convert a sub-typed object to a readable String | Unchecked, unsafe
template<template<typename ...> class C, typename T, typename Targ>
std::string show_c(std::function<std::string(Targ)> showF, const C<T>& x) = delete;

template<template<typename, typename> class C, typename T1, typename T2, typename Targ1, typename Targ2>
std::string show_c2(std::function<std::string(Targ1)> showF1, std::function<std::string(Targ2)> showF2, const C<T1, T2>& x) = delete;

template <typename T>
requires (std::is_arithmetic<T>::value) std::string show_t(T x) {
    return std::to_string(x);
};

template <typename T>
requires (std::convertible_to<T, std::string>) std::string show_t(T x) {
    return "\"" + static_cast<std::string>(x) + "\"";
};

template <template<typename ...> class C, typename T, typename Targ> 
requires (std::forward_iterator<typename C<T>::iterator>
         && (std::same_as<Targ, T> || std::same_as<Targ, const T&>)) 
std::string show_c(std::function<std::string(Targ)> showF, const C<T>& xs) {
    std::string s = "[";
    for (T x : xs) {
        s.append(showF(x) + ", ");
    };
    return s.substr(0, s.length() - 2)  + "]";
}

template<typename T, typename Targ> 
requires (std::same_as<Targ, T> || std::same_as<Targ, const T&>)
std::string show_c(std::function<std::string(Targ)> showF, T xs[]) {
    std::string s = "{";
    for (T x : xs) {
        s.append(showF(x) + ", ");
    };
    return s.substr(0, s.length() - 2)  + "}";
}

template<typename T1, typename T2, typename Targ1, typename Targ2>
requires ((std::same_as<Targ1, T1> || std::same_as<Targ1, const T1&>) && (std::same_as<Targ2, T2> || std::same_as<Targ2, const T2&>))
std::string show_c2(std::function<std::string(Targ1)> showF1, std::function<std::string(Targ2)> showF2, const std::pair<T1, T2>& x) {
    return "(" + showF1(std::get<0>(x)) + ", " + showF2(std::get<1>(x)) + ")";
}

#endif
