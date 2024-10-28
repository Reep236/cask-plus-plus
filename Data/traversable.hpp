#include <functional>

#include "traversable.func.hpp"
#include "traversable.conc.hpp"
#include "../Control/monad.hpp"

#ifndef TRAVERSABLE_HPP
#define TRAVERSABLE_HPP

template<template<typename ...> class Tr, template<typename ...> class F, typename T1, typename T2>
requires (Applicative<F, T2> && Traversable<Tr, T1>) F<Tr<T2>> traverse(std::function<F<T2>(T1)> f, Tr<T1> x) {
    return traverse_c(asF(pure<F, Tr<T2>>), 
                      asF(fmap<F, T2, Tr<T2>>), 
                      asF(liftA2<F, T2, Tr<T2>, Tr<T2>>), 
                      f, x);
} 

template<template<typename, typename> class Tr, template<typename ...> class F, typename Ta, typename T1, typename T2>
requires (Applicative<F, T2> && Traversable2<Tr, Ta, T1>) F<Tr<Ta, T2>> traverse(std::function<F<T2>(T1)> f, Tr<Ta, T1> x) {
    return traverse_c2(asF(pure<F, Tr<Ta, T2>>), 
                       asF(fmap<F, T2, Tr<Ta, T2>>), 
                       asF(liftA2<F, T2, Tr<Ta, T2>, Tr<Ta, T2>>), 
                       f, x);
} 

template<template<typename ...> class Tr, template<typename ...> class F, typename T>
requires (Applicative<F, T> && Traversable<Tr, T>) inline F<Tr<T>> sequenceA(Tr<F<T>> x) { 
    return traverse(id<F<T>>, x); 
} 

template<template<typename, typename> class Tr, template<typename ...> class F, typename Ta, typename T>
requires (Applicative<F, T> && Traversable2<Tr, Ta, T>) inline F<Tr<Ta, T>> sequenceA(Tr<Ta, F<T>> x) { 
    return traverse(id<F<T>>, x); 
}

template<template<typename ...> class T, template<typename ...> class M, typename Ta, typename Tb>
requires (Monad<M, Tb>) inline M<T<Tb>> mapM(std::function<M<Tb>(Ta)> f, T<Ta> t) { return traverse(f, t); }

template<template<typename ...> class T, template<typename ...> class M, typename Ta>
requires (Monad<M, Ta>) inline M<T<Ta>> sequence(T<M<Ta>> t) { return sequenceA(t); }

#endif
