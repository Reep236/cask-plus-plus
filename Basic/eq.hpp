#include "eq.func.hpp"
#include "eq.conc.hpp"

#ifndef EQ_HPP
#define EQ_HPP

template<typename T>
requires (Eq<T>) inline bool eq(T a, T b) { return eq_t(a, b); }

template<template<typename ...> class C, typename T>
requires (Eq<T> && Eq1<C, T>) inline bool eq(C<T> a, C<T> b) { return eq_c(asF(eq<T>), a, b); }

template<template<typename, typename> class C, typename T1, typename T2>
requires (Eq<T1> && Eq<T2> && Eq2<C, T1, T2>) inline bool eq(C<T1, T2> a, C<T1, T2> b) { 
    return eq_c2(asF(eq<T1>), asF(eq<T2>), a, b); 
}

template<class C>
requires (Eq<C>) inline bool operator==(C a, C b) { return eq_t(a, b); }

template<template<typename ...> class C, typename T>
requires (Eq<T> && Eq1<C, T>) inline bool operator==(C<T> a, C<T> b) { return eq_c(asF(eq<T>), a, b); }

template<template<typename, typename> class C, typename T1, typename T2>
requires (Eq<T1> && Eq<T2> && Eq2<C, T1, T2>) inline bool operator==(C<T1, T2> a, C<T1, T2> b) {
    return eq_c2(asF(eq<T1>), asF(eq<T2>), a, b); 
}

template<typename T>
inline bool neq(T a, T b) { return !eq(a, b); }

template<class C>
inline bool operator!=(C a, C b) { return !eq(a, b); }

template<template<typename ...> class C, typename T>
inline bool operator!=(C<T> a, C<T> b) { return !eq(a, b); }

template<template<typename, typename> class C, typename T1, typename T2>
inline bool operator!=(C<T1, T2> a, C<T1, T2> b) { return !eq(a, b); }

#endif
