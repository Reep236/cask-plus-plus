#include <concepts>

#include "traversable.func.hpp"
#include "../Basic/base.hpp"
#include "../Control/applicative.hpp"

#ifndef TRAVERSABLE_CONC_HPP
#define TRAVERSABLE_CONC_HPP

template<template<typename ...> class Tr, typename T>
concept Traversable = requires (Tr<T> x) {
    { traverse_c(asF(pure<std::list, Tr<T>>), 
                 asF(fmap<std::list, T, Tr<T>>), 
                 asF(liftA2<std::list, T, Tr<T>, Tr<T>>), 
                 asF(pure<std::list, T>), x) };
};

template<template<typename, typename> class Tr, typename Ta, typename T>
concept Traversable2 = requires (Tr<Ta, T> x) {
    { traverse_c2(asF(pure<std::list, Tr<Ta,T>>), 
                  asF(fmap<std::list, T, Tr<Ta,T>>), 
                  asF(liftA2<std::list, T, Tr<Ta,T>, Tr<Ta,T>>),
                  asF(pure<std::list, T>), x) };
};

#endif
