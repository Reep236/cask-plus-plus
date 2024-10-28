#include <stack>

#ifndef LIST_FUNC_HPP
#define LIST_FUNC_HPP

template<template<typename ...> class L, typename T>
concept _ListBuiltin = requires (L<T> xs) {
    {xs.push_front(xs.front())};
    {xs.pop_front()};
};

template<template<typename ...> class L, typename T>
concept _SequenceBuiltin = requires (L<T> xs) {
    {xs.insert(xs.end(), xs.front())};
    {xs.push_back(xs.back())};
};

// All functions initially defined in-place for efficiency, LIST_HPP will provide safe wrappers 

// Requirements of ListLike, defaults using common syntax:
template<template<typename ...> class L, typename T>
requires (_ListBuiltin<L, T>) inline L<T> prepend_f(T x, L<T>& xs) { xs.push_front(x); return xs; }

template<template<typename ...> class L, typename T>
requires (_ListBuiltin<L, T>) inline T head_f(const L<T>& xs) { return xs.front(); }

template<template<typename ...> class L, typename T>
requires (_ListBuiltin<L, T>) inline L<T> tail_f(L<T>& xs) { xs.pop_front(); return xs; }

template<template<typename ...> class L, typename T>
inline bool null_t(const L<T>& xs) { return xs.empty(); }

template<template<typename ...> class L, typename T>
inline L<T> singleton_t(T x) { return L<T>{x}; }

template<typename T>
inline std::stack<T> prepend_f(T x, std::stack<T>& xs) { xs.push(x); return xs; }

template<typename T>
inline T head_f(const std::stack<T>& xs) { return xs.top(); }

template<typename T>
inline std::stack<T> tail_f(std::stack<T>& xs) { xs.pop(); return xs; }

// Requirements of Containers, in addition to the mutual requirements of singleton_t and null_t:
template<template<typename ...> class L, typename T>
requires(_SequenceBuiltin<L, T>) L<T> prepend_t(T x, L<T>& xs) { xs.insert(xs.begin(), x); return xs; }

template<template<typename ...> class L, typename T>
requires(_SequenceBuiltin<L, T>) T head_t(const L<T>& xs) { return xs.front(); }

template<template<typename ...> class L, typename T>
requires(_SequenceBuiltin<L, T>) L<T> tail_t(L<T>& xs) { xs.erase(xs.begin()); return xs; }

template<template<typename ...> class L, typename T>
requires(_SequenceBuiltin<L, T>) L<T> append_t(T x, L<T>& xs) { xs.push_back(x); return xs; }

template<template<typename ...> class L, typename T> 
requires(_SequenceBuiltin<L, T>) T last_t(const L<T>& xs) { return xs.back(); }

template<template<typename ...> class L, typename T> 
requires(_SequenceBuiltin<L, T>) L<T> init_t(L<T>& xs) { xs.pop_back(); return xs; }

template<template<typename ...> class L, typename T>
requires(_SequenceBuiltin<L, T>) L<T> cat_t(L<T>& ys, const L<T>& xs) { ys.insert(ys.end(), xs.begin(), xs.end()); return ys; }

#endif
