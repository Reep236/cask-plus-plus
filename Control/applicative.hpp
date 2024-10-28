#include "applicative.func.hpp"
#include "applicative.conc.hpp"
#include "../Data/list.hpp"

#ifndef APPLICATIVE_HPP
#define APPLICATIVE_HPP

// Lift a value into a structure
template<template<typename...> class A, typename T>
requires (Applicative<A,T>) inline A<T> pure(T a) { return pure_c((A<T>*)nullptr, a); }

// Sequential application 
template<template<typename...> class A, typename  Ta, typename Tb>
requires (Applicative<A,Ta>) inline A<Tb> ap(A<std::function<Tb(Ta)>> f, A<Ta> a) { return ap_c(f, a); }

template<template<typename, typename> class A, typename Ta, typename T>
requires (Applicative2<A, Ta, T>) inline A<Ta, T> pure(T a) { return pure_c2((A<Ta, T>)nullptr, a); }

template<template<typename, typename> class A, typename Ta, typename T1, typename T2>
requires (Applicative2<A, Ta, T1>) inline A<Ta, T2> ap(A<Ta, std::function<T2(T1)>> f, A<Ta, T1> a) { return ap_c2(f, a); }

// Alternative
template<template<typename ...> class A, typename T>
requires (Alternative<A, T>) inline const A<T> emptyA = _emptyA<A<T>>;

template<template<typename ...> class A, typename Ta, typename Tb>
requires (Alternative<A, Ta>) inline A<Tb> lazyAp(A<std::function<Ta(Tb)>> f, std::function<A<Ta>()> a) { return lazyAp_c(f, a); }

template<template<typename...> class A, typename T>
requires (Alternative<A, T>) inline A<T> aplus(A<T> a, const A<T>& as) { return aplus_c(a, as); }

// Lift a binary function to actions
template<template<typename...> class A, typename Ta, typename Tb, typename Tc>
A<Tc> liftA2(std::function<Tc(Ta, Tb)> f, A<Ta> aa, A<Tb> ab) {
    std::function<std::function<Tc(Tb)>(Ta)> f2 = [f] (Ta a) {return std::bind_front(f, a);};
    A<std::function<Tc(Tb)>> f3 = fmap(f2, aa);  
    return ap(f3, ab); 
};

template<template<typename, typename> class A, typename Ta, typename T1, typename T2, typename T3>
A<Ta, T3> liftA2(std::function<T3(T1, T2)> f, A<Ta, T1> aa, A<Ta, T2> ab) {
    std::function<std::function<T3(T2)>(T1)> f2 = [f] (T1 a) {return std::bind_front(f, a);};
    A<Ta, std::function<T3(T2)>> f3 = fmap(f2, aa);  
    return ap(f3, ab); 
};

template<template<typename ...> class A, typename Ta, typename Tb, typename Tc>
A<Tc> lazyA2(std::function<Tc(Ta, Tb)> f, A<Ta> aa, std::function<A<Tb>()> bf) {
    std::function<std::function<Tc(Tb)>(Ta)> f2 = [f] (Ta a) {return partial(f, a);};
    A<std::function<Tc(Tb)>> f3 = fmap(f2, aa);
    return lazyAp(f3, bf);
}

// Sequence actions, discarding the value of the first argument.
template<template<typename...> class A, typename Ta, typename Tb>
inline A<Tb> ignoreA(A<Ta> a, A<Tb> b) {
    return ap(fconst(id<Tb>, a), b);
}

template<template<typename, typename> class A, typename Ta, typename T1, typename T2>
inline A<Ta, T2> ignoreA(A<Ta, T1> a, A<Ta, T2> b) {
    return ap(fconst(id<T2>, a), b);
}

// Sequence actions, discarding the value of the second argument.
template<template<typename...> class A, typename Ta, typename Tb>
A<Tb> ignoreB(A<Ta> a, A<Tb> b) {
    return liftA2(asF(constf<Ta, Tb>), b, a); 
}

template<template<typename, typename> class A, typename Ta, typename T1, typename T2>
A<Ta, T2> ignoreB(A<Ta, T1> a, A<Ta, T2> b) {
    return liftA2(asF(constf<T1, T2>), a, b); 
}


// Alternative:

template<template<typename ...> class L, template<typename ...> class A, typename T>
A<L<T>> many(A<T> v) {
    return aplus(lazyA2(asF(cons<L, T>), v, partial(many<L, A, T>, v)), pure<A>(L<T>()));
}

template<template<typename ...> class L, template<typename ...> class A, typename T>
A<L<T>> some(A<T> v) {
    return lazyA2(asF(cons<L, T>), 
                  v, 
                  partial(many<L, A, T>, v));
}  

#endif
