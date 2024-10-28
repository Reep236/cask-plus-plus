#include "../Data/functor.hpp"
#include "applicative.func.hpp"

#ifndef APPLICATIVE_CONC_HPP
#define APPLICATIVE_CONC_HPP

// A functor with application, providing operations to 
//  - embed pure expressions (`pure`), and 
//  - sequence computations and combine their results (`ap` and `liftA2`)
template<template<typename...> class A, typename T>
concept Applicative = Functor<A, T> && requires(A<T> x) {
    { ap_c(pure_c((A<std::function<T(T)>>*)nullptr, id<T>), x) };
};

template<template<typename...> class A, typename T>
concept Alternative = Applicative<A, T> && requires(A<T> x) {
    { aplus_c(_emptyA<A<T>>, x) };
    { lazyAp_c(pure_c((A<std::function<T(T)>>*)nullptr, id<T>), partial(id<A<T>>, x))};
};

template<template<typename, typename> class A, typename Ta, typename T>
concept Applicative2 = Functor2<A, Ta, T> && requires(A<Ta, T> x) {
    { ap_c2(pure_c2((A<Ta,std::function<T(T)>>*)nullptr, id<T>), x) };
};

#endif
