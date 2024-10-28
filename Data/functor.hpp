#include "functor.func.hpp"
#include "functor.conc.hpp"
#include "../Basic/base.hpp"

#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

template<template<typename ...> class F, typename T1, typename T2> 
requires (Functor<F, T1>) inline F<T2> fmap(std::function<T2(T1)> f, F<T1> x) { return fmap_c(f, x); }

template<template<typename, typename> class F, typename Ta, typename T1, typename T2>
requires (Functor2<F, Ta, T1>) inline F<Ta, T2> fmap(std::function<T2(T1)> f, F<Ta, T1> x) { return fmap_c2(f, x); }

// Replace all locations in the input with the same value
template<template<typename...> class F, typename Ta, typename Tb>
requires (Functor<F, Ta>) F<Ta> fconst(Ta a, F<Tb> fb) {
    return fmap(constf<Tb>(a), fb);
};

template<template<typename, typename> class F, typename Ta, typename T1, typename T2>
requires (Functor2<F, Ta, T1>) F<Ta, T1> fconst(T1 x, F<Ta, T2> f2) {
    return fmap(constf<T2>(x), f2);
};

#endif
