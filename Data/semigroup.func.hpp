#include <forward_list>
#include <functional>
#include <list>
#include <string>
#include <vector>
#include <deque>
#include <unordered_set>
#include <set>

#ifndef SEMIGROUP_FUNC_HPP
#define SEMIGROUP_FUNC_HPP

// An associative operation 
template<typename M>
M sappend_t(M m, M ms) = delete;

// An associative operation
template<template<typename ...> class M, typename T>
M<T> sappend_c(std::function<T(T,T)> sappendF, M<T> m, M<T> ms) = delete;

template<typename T>
std::vector<T> sappend_t(std::vector<T> m, std::vector<T> ms) {
    ms.insert(ms.end(), m.begin(), m.end());
    return ms;
};

template<typename T>
std::deque<T> sappend_t(std::deque<T> m, std::deque<T> ms) {
    ms.insert(ms.end(), m.begin(), m.end());
    return ms;
};

template<typename T>
std::forward_list<T> sappend_t(std::forward_list<T> m, std::forward_list<T> ms) {
    ms.splice_after(ms.end(), m);
    return ms;
};

template<typename T>
std::list<T> sappend_t(std::list<T> m, std::list<T> ms) {
    ms.splice(ms.end(), m);
    return ms;
};

template<typename T>
std::unordered_set<T> sappend_t(std::unordered_set<T> m, std::unordered_set<T> ms) {
    ms.merge(m);
    return ms;
}

template<typename T>
std::set<T> sappend_t(std::set<T> m, std::set<T> ms) {
    ms.merge(m);
    return ms;
}

std::string sappend_t(std::string, std::string);

#endif
