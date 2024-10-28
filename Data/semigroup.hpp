#include "semigroup.func.hpp"
#include "semigroup.conc.hpp"

#ifndef SEMIGROUP_HPP
#define SEMIGROUP_HPP

template<typename T>
requires (Semigroup<T>) inline T sappend(T m, T ms) { return sappend_t(m, ms); }

template<template<typename ...> class C, typename T>
requires (Semigroup1<C, T>) inline C<T> sappend(C<T> m, C<T> ms) { 
    return sappend_c(asF(static_cast<T(*)(T,T)>(sappend)), m, ms); 
}

#endif
