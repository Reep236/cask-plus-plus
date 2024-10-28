#include "ord.func.hpp"
#include "ord.conc.hpp"

#ifndef ORD_HPP
#define ORD_HPP

template<typename T> 
requires (Ord<T>) inline bool lte(T a, T b) { return lte_t(a, b); }

template<template<typename ...> class C, typename T>
requires (Ord1<C, T>) inline bool lte(C<T> a, C<T> b) { return lte_c(asF(lte<T>), a, b); } 

template<template<typename ...> class C1, template<typename ...> class C2, typename T>
requires (Ord1<C1, T>, Ord1<C2, T>) inline bool lte(C1<C2<T>> a, C1<C2<T>> b) { return lte_c(asF(lte<C2, T>), a, b); } 

template<template<typename, typename> class C, typename T1, typename T2>
requires (Ord2<C, T1, T2>) inline bool lte(C<T1, T2> a, C<T1, T2> b) { 
    return lte_c2(asF(lte<T1>), asF(lte<T2>), a, b); 
} 

template<class C> 
inline bool operator<=(C a, C b) { return lte(a, b); }

template<template<typename ...> class C, typename T>
inline bool operator<=(C<T> a, C<T> b) { return lte(a, b); } 

template<typename T>
Ordering compare(T a, T b) { 
    if (eq(a, b)) { return Ordering::EQ; }
    else if (lte(a, b)) { return Ordering::LT; }
    else { return Ordering::GT; }
}

template<typename T> 
inline bool gte(T a, T b) { return lte(b, a); }

template<typename T>
inline bool gt(T a, T b) { return !(lte(a, b)); } 

template<typename T>
inline bool lt(T a, T b) { return !(lte(b, a)); }

template<class C> 
inline bool operator>=(C a, C b) { return lte(b, a); }

template<class C>
inline bool operator>(C a, C b) { return !(lte(a, b)); } 

template<class C>
inline bool operator<(C a, C b) { return !(lte(b, a)); }

template<template<typename ...> class C, typename T> 
inline bool operator>=(C<T> a, C<T> b) { return lte(b, a); }

template<template<typename ...> class C, typename T>
inline bool operator>(C<T> a, C<T> b) { return !(lte(a, b)); } 

template<template<typename ...> class C, typename T>
inline bool operator<(C<T> a, C<T> b) { return !(lte(b, a)); }

template<typename T>
inline T max(T a, T b) { return lte(a, b) ? b:a; }

template<typename T>
inline T min(T a, T b) { return lte(a, b) ? a:b; }

template<typename T>
inline T clamp(T low, T high, T arg) { return min(high, max(arg, low)); }

#endif
