#include "eq.func.hpp"

#ifndef EQ_CONC_HPP
#define EQ_CONC_HPP

template<typename T>
concept Eq = requires (T x) {
    { eq_t(x, x)  };
};

template<template<typename ...> class C, typename T> 
concept Eq1 = !Eq<C<T>> && requires (C<T> x) {
    { eq_c(asF(eq_t<T>), x, x) };
};

template<template<typename, typename> class C, typename T1, typename T2>
concept Eq2 = !Eq<C<T1, T2>> && requires (C<T1, T2> x) {
    { eq_c2(asF(eq_t<T1>), asF(eq_t<T2>), x, x) };
};

#endif
