#include <functional>
#include <list>
#include <vector>

#ifndef MONAD_FUNC_HPP
#define MONAD_FUNC_HPP
template<template<typename ...> class M, typename Ta, typename Tb>
M<Tb> constbind(M<Ta> ma, M<Tb> mb) = delete;

// `requires(false)` places out of the running in overload resolution where `= delete` doesn't
template<template<typename ...> class M, typename Ta, typename Tb> 
requires(false) M<Tb> bind_c(M<Ta> a, std::function<M<Tb>(Ta)> f) = delete; 

template<template<typename, typename> class M, typename Ta, typename T1, typename T2>
M<Ta, T2> bind_c2(M<Ta, T1> a, std::function<M<Ta, T2>(T1)> f) = delete; 

template<typename Ta, typename Tb>
std::list<Tb> bind_c(std::list<Ta> xs, std::function<std::list<Tb>(Ta)> f) {
    std::list<Tb> ys;
    for (Ta x : xs) ys.splice(ys.end(), f(x));
}

template<typename Ta, typename Tb>
std::vector<Tb> bind_c(std::vector<Ta> xs, std::function<std::vector<Tb>(Ta)> f) {
    std::vector<Tb> ys;
    for (Ta x : xs) {
        std::vector v = f(x);
        ys.insert(ys.end(), v.begin(), v.end());
    }
    return ys;
}

#endif
