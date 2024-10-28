#include "../Data/monoid.func.hpp"
#include "../IO/show.hpp"

#ifndef ORD_FUNC_HPP
#define ORD_FUNC_HPP

enum class Ordering {LT, EQ, GT};
std::string show_t(Ordering x);
Ordering read_(Ordering* phantom, std::string x);

template<>
inline const Ordering _mempty<Ordering> = Ordering::EQ;

Ordering mappend_t(Ordering m, Ordering ms);

template<typename T> 
inline bool lte_t(T a, T b) { return a <= b; };

template<template<typename ...> class C, typename T>
inline bool lte_c(std::function<bool(T,T)>, C<T> a, C<T> b) { return a <= b; };

template<template<typename, typename> class C, typename T1, typename T2>
inline bool lte_c2(std::function<bool(T1,T1)>, std::function<bool(T2,T2)>, C<T1, T2> a, C<T1, T2> b) { return a <= b; };

#endif
