#include "semigroup.func.hpp"

#ifndef SEMIGROUP_CONC_HPP
#define SEMIGROUP_CONC_HPP

template<typename T>
concept Semigroup = requires (T x) {
    { sappend_t(x, x) }; 
};

template<template<typename ...> class C, typename T>
concept Semigroup1 = !Semigroup<C<T>> && requires (C<T> x) {
    { sappend_c(asF(static_cast<T(*)(T,T)>(sappend_t)), x, x)};
};

#endif
