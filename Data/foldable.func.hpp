#include <functional>
#include <ranges>

#ifndef FOLDABLE_FUNC_HPP
#define FOLDABLE_FUNC_HPP

template<template<typename...> class T, typename Ta, typename Tb>
Tb foldr_c(std::function<Tb(Ta, Tb)> f, Tb z, T<Ta> t) = delete;

template<template<typename, typename> class T, typename Ta, typename T1, typename T2>
T2 foldr_c2(std::function<T2(T1, T2)> f, T2 z, T<Ta, T1> t) = delete;

template<template<typename...> class T, typename Ta, typename Tb>
Tb foldl(std::function<Tb(Ta, Tb)> f, Tb z, T<Ta> t) = delete;

template<template<typename ...> class T, typename Ta>
inline size_t length(T<Ta> t) = delete;

template<template<typename ...> class T, typename Ta>
requires (std::ranges::sized_range<T<Ta>>) size_t length(T<Ta> t) {
    return std::ranges::size(t);
} 

template<template<typename ...> class T, typename Ta, typename Tb>
requires (std::ranges::bidirectional_range<T<Ta>>) Tb foldr_c(std::function<Tb(Ta, Tb)> f, Tb z, T<Ta> as) {
    Tb res = z; 
    std::ranges::reverse_view rvas{as};
    for (auto a : rvas) {
        res = f(a, res);
    }
    return res;
}

template<template<typename ...> class T, typename Ta, typename Tb>
requires (std::forward_iterator<typename T<Ta>::iterator>) Tb foldl(std::function<Tb(Tb, Ta)> f, Tb z, T<Ta> as) {
    Tb res = z; 
    for (auto a : as) {
        res = f(res, a);
    }
    return res;
}

template<template<typename ...> class T, typename Ta, typename Tb>
requires (std::ranges::forward_range<T<Ta>>) Tb foldr_c(std::function<Tb(Ta, Tb)> f, Tb z, T<Ta> as) {
    typename T<Ta>::iterator endP = as.end();
    std::function<Tb(typename T<Ta>::iterator)> innerF = 
                [&f,z,endP,&innerF] (typename T<Ta>::iterator a) -> Tb { return a == endP ? z:f(*a, innerF(next(a))); };
    return innerF(as.begin());
}

#endif
