#ifndef EQ_FUNC_HPP
#define EQ_FUNC_HPP

// Alias of (==)
#include <functional>

template<typename T>
inline bool eq_t(T a, T b) { return a == b; }

template<template<typename ...> class C, typename T> 
inline bool eq_c(std::function<bool(T,T)>, C<T> a, C<T> b) { return a == b; }

template<template<typename, typename> class C, typename T1, typename T2>
inline bool eq_c2(std::function<bool(T1,T1)>, std::function<bool(T2,T2)>, C<T1, T2> a, C<T1, T2> b) { return a == b; }

#endif

