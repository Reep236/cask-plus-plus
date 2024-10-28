#include "enum.func.hpp"
#include "enum.conc.hpp"
#include "../Data/list.hpp"
#include <list>
#include <stdexcept>

#ifndef ENUM_HPP
#define ENUM_HPP

template<typename T>
requires (Bounded<T>) constexpr T minBound = minBound_t<T>;

template<typename T>
requires (Bounded<T>) constexpr T maxBound = maxBound_t<T>;

template<typename T>
requires (Enum<T>) inline int fromEnum(T x) { return fromEnum_t(x); }

// TODO: Add check with int bounds, for user-defined non-standard enums 
// template<typename T>
// requires (Enum<T> && Bounded<T>) int fromEnum(T x) { return fromEnum_t(x); }

template<typename T>
requires (Enum<T>) inline T toEnum(int i) { return toEnum_t((T*)nullptr, i); }

template<typename T>
requires (Enum<T> && Bounded<T>) T toEnum(int i) { 
    if ((fromEnum_t(minBound<T>) <= i) && (i <= fromEnum_t(maxBound<T>))) 
        return toEnum_t((T*)nullptr, i); 
    throw std::invalid_argument("Argument of toEnum out of bounds");
}

template<typename T>
inline T succ(T x) { return toEnum<T>(fromEnum(x) + 1); }

template<typename T>
requires (std::is_arithmetic<T>::value) inline T succ(T x) { return x + 1; }

template<typename T>
requires (Bounded<T> && std::is_arithmetic<T>::value) inline T succ(T x) {
    return x < maxBound<T> ? (x + 1):(throw std::invalid_argument("Illegal successor of maxBound"));
}

template<typename T>
inline T pred(T x) { return toEnum<T>(fromEnum(x) - 1); }

template<typename T>
requires (std::is_arithmetic<T>::value) inline T pred(T x) { return x - 1; }

template<typename T>
requires (Bounded<T> && std::is_arithmetic<T>::value) inline T pred(T x) {
    return x > minBound<T> ? (x - 1):(throw std::invalid_argument("Illegal predecessor of minBound"));
}

template<template<typename ...> class L = std::list, typename T>
requires (Sequence<L, T>) L<T> enumFromTo(T x, T y) { 
    L<T> ys;
    for (int i = fromEnum(x); i <= fromEnum(y); ++i)
        append(toEnum<T>(i), ys);
    return ys;
}

template<template<typename ...> class L = std::list, typename T>
L<T> enumFromTo(T x, T y) { 
    L<T> ys;
    for (int i = fromEnum(y); i >= fromEnum(x); --i)
        prepend(toEnum<T>(i), ys);
    return ys;
}

template<template<typename ...> class L = std::list, typename T>
requires (Sequence<L, T>) L<T> enumFromThenTo(T x1, T x2, T y) {
    int i1 = fromEnum(x1);
    int i2 = fromEnum(x2);
    L<T> ys;
    for (int i = i1; i <= fromEnum(y); i += i2 - i1)
        append(toEnum<T>(i), ys);
    return ys; 
}

template<template<typename ...> class L = std::list, typename T>
L<T> enumFromThenTo(T x1, T x2, T y) {
    int i1 = fromEnum(x1);
    int i2 = fromEnum(x2);
    int iy = fromEnum(y);

    int diff = i1 - i2;
    int ceil = iy - (iy - i1)%diff;

    L<T> ys;
    for (int i = ceil; i >= i1; i -= diff)
        prepend(toEnum<T>(i), ys);

    return ys; 
}

template<template<typename ...> class L = std::list, typename T>
requires(Bounded<T>) 
[[deprecated("enumFrom() is eager and performs poorly with large bounds")]]
inline L<T> enumFrom(T x) {
    return enumFromTo<L>(x, maxBound<T>);
} 

template<template<typename ...> class L = std::list, typename T>
requires (Bounded<T>) 
[[deprecated("enumFromThen() is eager and performs poorly with large bounds")]]
inline L<T> enumFromThen(T x1, T x2) {
    return enumFromThenTo<L>(x1, x2, maxBound<T>);
}

#endif
