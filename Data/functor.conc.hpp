#include <concepts>
#include "../Basic/base.func.hpp"
#include "functor.func.hpp"

#ifndef FUNCTOR_CONC_HPP
#define FUNCTOR_CONC_HPP

// A type `F` is a `Functor` if it provides a function `fmap` which, given any types `a` and `b`, 
// lets you apply any function from `(a -> b)` to turn an `f a` into an `f b`, preserving the structure of `f`.
template<template<typename...> class F, typename T>
concept Functor = requires (F<T> x) {
        { fmap_c(id<T>, x) };
};

template<template<typename, typename> class F, typename Ta, typename T>
concept Functor2 = requires (F<Ta, T> x) {
        { fmap_c2(id<T>, x) };
};


#endif
