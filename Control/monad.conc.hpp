#include "applicative.hpp"

#ifndef MONAD_CONC_HPP
#define MONAD_CONC_HPP

template<template<typename...> class M, typename T>
concept Monad = Applicative<M, T> && requires (M<T> x) {
    { bind_c(x, asF(static_cast<M<T>(*)(T)>(pure))) };
};

template<template<typename, typename> class M, typename Ta, typename T>
concept Monad2 = Applicative2<M, Ta, T> && requires (M<Ta, T> x) {
    { bind_c2(x, asF(pure<M, Ta, T>)) };
};
#endif
