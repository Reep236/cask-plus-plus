#include <vector>
#include <functional>

#include "../Data/list.func.hpp"

#ifndef APPLICATIVE_FUNC_HPP
#define APPLICATIVE_FUNC_HPP

// Lift value `x` into structure `L`
template<template<typename ...> class L, typename T>
requires (_SequenceBuiltin<L, T>) inline L<T> pure_c(L<T>*, T x) { return L<T>{x}; }

// Sequential application 
// Apply all functions in `fs` to all values in `as` by Cartesian product
template<template<typename ...> class L, typename Ta, typename Tb> 
requires (_SequenceBuiltin<L, Ta>) L<Tb> ap_c(L<std::function<Tb(Ta)>> fs, L<Ta> as) {
    L<Tb> bs;
    for (auto f : fs) {
        for (auto a : as) {
            bs.push_back(f(a));
        }
    }
    return bs;
}

template<template<typename ...> class L, typename Ta, typename Tb> 
requires (_SequenceBuiltin<L, Ta>) L<Tb> lazyAp_c(L<std::function<Tb(Ta)>> fs, std::function<L<Ta>()> as) {
    L<Tb> bs;
    for (auto f : fs) {
        for (auto a : as()) {
            bs.push_back(f(a));
        }
    }
    return bs;
}

template<template<typename, typename> class C, typename Ta, typename T>
C<Ta, T> pure_c2(C<Ta, T>*, T) = delete;

template<template<typename, typename> class C, typename Ta, typename T1, typename T2>
C<Ta, T2> ap_c2(C<Ta, std::function<T2(T1)>>, C<Ta, T1>) = delete;

// For Alternative:

// The identity of `aplus`
template<typename T>
inline const T _emptyA;

template<template<typename ...> class L, typename T>
requires (_SequenceBuiltin<L, T>) inline const L<T> _emptyA<L<T>> = L<T>();

// An associative binary operation
template<template<typename ...> class L, typename T>
requires (_SequenceBuiltin<L, T>) inline L<T> aplus_c(L<T> a, const L<T>& b) {
    return a.insert(a.end(), b.begin(), b.end());
}

template<typename Ta, typename Tb>
std::vector<Tb> ap_c(std::vector<std::function<Tb(Ta)>> fs, std::vector<Ta> as) {
    std::vector<Tb> bs;
    bs.reserve(as.capacity()*fs.capacity());
    for (auto f : fs)
        for (auto a : as)
            bs.push_back(f(a));
    return bs;
}

template<typename Ta, typename Tb>
std::vector<Tb> lazyAp_c(std::vector<std::function<Tb(Ta)>> fs, std::function<std::vector<Ta>()> asF) {
    if (fs.size() == 0) return std::vector<Tb>();

    std::vector<Tb> bs;
    std::vector<Ta> as = asF();
    
    bs.reserve(as.size()*fs.size());
    for (auto f : fs)
        for (auto a : as)
            bs.push_back(f(a));
    return bs;
}

#endif
