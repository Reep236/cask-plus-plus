#include <cstddef>
#include <functional>
#include <list>
#include <array>
#include <unordered_set>
#include <vector>

#ifndef FUNCTOR_FUNC_HPP
#define FUNCTOR_FUNC_HPP

// `fmap_c` is used to apply a function of type `(a -> b)` to a value of type `f a`, where `f` is a functor, to produce a value of type `f b`
// Note that for any type constructor with more than one parameter (e.g. `Either`), only the last type parameter can be modified with `fmap_c` 
// (e.g., `b` in `Either a b`).
//
// Some type constructors with two parameters or more have a `Bifunctor` insteance that allows the last and the penultimate parameters to be mapped over
// TODO: Implement `Either`, `Bifunctor` 
template<template<typename ...> class F, typename T1, typename T2> 
F<T2> fmap_c(std::function<T2(T1)>, F<T1>) = delete;

template<template<typename, typename> class F, typename Ta, typename T1, typename T2> 
F<Ta, T2> fmap_c2(std::function<T2(T1)>, F<Ta, T1>) = delete;

// Map `f` over every element of `xs` 
template<typename T1, typename T2>
std::list<T2> fmap_c(std::function<T2(T1)> f, std::list<T1> xs) {
    std::list<T2> ys;
    for (T1 x : xs) {
        ys.emplace_back(f(x));
    }
    return ys;
}

// Map `f` over every element of `xs`
// Templating on array size may lead to bloat
template<typename T1, typename T2, std::size_t sz>
std::array<T2, sz> fmap_c(std::function<T2(T1)> f, T1 (&xs)[sz]) {
    T2 ys[sz];
    for (int idx = 0; idx < sz; idx++) {
        ys[idx] = f(xs[idx]);
    }
    return ys; 
}

template<typename T1, typename T2>
std::vector<T2> fmap_c(std::function<T2(T1)> f, std::vector<T1> xs) {
    std::vector<T2> ys;
    ys.reserve(xs.capacity());
    for (T1 x : xs)
        ys.emplace_back(f(x));
    return ys;
}

template<typename T1, typename T2>
std::unordered_set<T2> fmap_c(std::function<T2(T1)> f, std::unordered_set<T1> xs) {
    std::unordered_set<T2> ys;
    for (auto x : xs) {
        ys.insert(f(x));
    }
    return ys;
}

template<typename Ta, typename T1, typename T2>
std::pair<Ta, T2> fmap_c2(std::function<T2(T1)> f, std::pair<Ta, T1> xs) {
    return std::pair<Ta, T2>(std::get<0>(xs), f(std::get<1>(xs)));
}
#endif
