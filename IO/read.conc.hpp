#include "read.func.hpp"
#include "show.hpp"

#ifndef READ_CONC_HPP
#define READ_CONC_HPP

template<typename T>
concept Read = Show<T> && requires (T x) {
    { read_t(&x, show(x)) } -> std::same_as<T>;
};

template<template<typename ...> class C, typename T>
concept Read1 = !Read<C<T>> && Show1<C, T> && requires (C<T> x) {
    { read_c(asF<T,T*,const std::string&>([] (T* phantom, const std::string& si) -> T {return read_t(phantom, si); }), &x, "") }; 
};

template<template<typename, typename> class C, typename T1, typename T2> 
concept Read2 = !Read<C<T1, T2>> && Show2<C, T1, T2> && Read<T1> && Read<T2> && requires (C<T1, T2> x) {
    { read_c2(asF<T1, T1*, const std::string&>(read_t<T1>), asF<T2, T2*, const std::string&>(read_t<T2>),
                 &x, "") };
};

#endif
