#include <list>
#include "foldable.func.hpp"
#include "../Basic/base.hpp"

#ifndef FOLDABLE_CONC_HPP
#define FOLDABLE_CONC_HPP

template<template<typename...> class T, typename Targ>
concept Foldable = requires(T<Targ> t) {
    {foldr_c(asF<std::list<Targ>, Targ, std::list<Targ>>([] (Targ a, std::list<Targ> as) {as.emplace_back(a); return as;}), 
             std::list<Targ> {}, t)};
};

template<template<typename, typename> class T, typename Ta, typename Targ>
concept Foldable2 = requires(T<Ta, Targ> t) {
    {foldr_c2(asF<std::list<Targ>, Targ, std::list<Targ>>([] (Targ a, std::list<Targ> as) {as.emplace_back(a); return as;}), 
              std::list<Targ> {}, t)};
};

#endif
