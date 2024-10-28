#include "show.func.hpp"
#include "../Basic/base.hpp"

#ifndef SHOW_CONC_HPP
#define SHOW_CONC_HPP

template<typename T>
concept Show = requires(T x) {
    { show_t(x) };
};

// Lambda required because Clang yaps about show_t<T> not fulfilling std::function<std::basic_string<char>(int)> 
template<template<typename ...> class C, typename T>
concept Show1 = !Show<C<T>> && Show<T> && requires (C<T> x) {
    { show_c(asF<std::string, T>(show_t<T>), x)  };
};

template<template<typename, typename> class C, typename T1, typename T2>
concept Show2 = !Show<C<T1, T2>> && Show<T1> && Show<T2> && requires (C<T1, T2> x) { 
    { show_c2(asF<std::string, T1>(show_t<T1>),
              asF<std::string, T2>(show_t<T2>), x) };
};

#endif
