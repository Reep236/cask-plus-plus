#include "show.func.hpp"
#include "show.conc.hpp"

#ifndef SHOW_HPP
#define SHOW_HPP

// Convert a value to a readable string
template <typename T>
requires (Show<T>) std::string show(T x) { return show_t(x); }

template <template<typename ...> class C, typename T>
requires (Show1<C, T>) std::string show(const C<T>& x) { return show_c(asF(show<T>), x); } 

template <template<typename ...> class C1, template<typename ...> class C2, typename T> 
requires (Show1<C1, T> && Show1<C2, T>) std::string show(const C1<C2<T>>& x) { return show_c(asF(show<C2, T>), x); }

template <template<typename ...> class C1, template<typename ...> class C2, template<typename ...> class C3, typename T> 
requires (Show1<C1, T> && Show1<C2, T> && Show1<C3, T>) std::string show(const C1<C2<C3<T>>>& x) { 
    return show_c(asF(show<C2, C3, T>), x); 
}

template <template<typename, typename> class C, typename T1, typename T2>
requires (Show2<C, T1, T2>) std::string show(const C<T1, T2>& x) { 
    return show_c2(asF(show<T1>), asF(show<T2>), x);
}


template <template<typename, typename> class CS, template<typename ...> class C1, typename T1, typename T2>
requires (!std::same_as<T2, std::allocator<C1<T1>>> && Show1<C1, T1> && Show2<CS, T1, T2>) std::string show(const CS<C1<T1>, T2>& x) { 
    return show_c2(asF(show<C1, T1>), asF(show<T2>), x);
}


template <template<typename, typename> class CS, template<typename ...> class C2, typename T1, typename T2>
requires (Show1<C2, T2> && Show2<CS, T1, T2>) std::string show(const CS<T1, C2<T2>>& x) { 
    return show_c2(asF(show<T1>), asF(show<C2, T2>), x);
}

template <template<typename, typename> class CS, template<typename ...> class C1, template<typename ...> class C2, typename T1, typename T2>
requires (Show1<C1, T1> && Show1<C2, T2> && Show2<CS, T1, T2>) std::string show(const CS<C1<T1>, C2<T2>>& x) { 
    return show_c2(asF(show<C1, T1>), asF(show<C2, T2>), x);
}

template<template<typename, typename> class CS, template<typename ...> class C, typename T1, typename T2>
requires (Show1<C, T1> && Show2<CS, T1, T2>) std::string show(const C<CS<T1, T2>>& x) {
    return show_c(asF(show<CS, T1, T2>), x);
}

#endif
