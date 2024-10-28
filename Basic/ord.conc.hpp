#include "ord.func.hpp"
#include "eq.hpp"

#ifndef ORD_CONC_HPP
#define ORD_CONC_HPP

template<typename T>
concept Ord = Eq<T> && requires (T x) {
    { lte_t(x, x) }; 
};

template<template<typename ...> class C, typename T>
concept Ord1 = !Ord<C<T>> && Eq1<C, T> && requires (C<T> x) {
    { lte_c(asF(lte_t<T>), x, x) };
};

template<template<typename, typename> class C, typename T1, typename T2>
concept Ord2 = !Ord<C<T1, T2>> && Eq2<C, T1, T2> && requires (C<T1, T2> x) {
    { lte_c2(asF(lte_t<T1>), asF(lte_t<T2>), x, x) };
};

#endif 
