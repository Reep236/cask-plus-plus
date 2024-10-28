#include <stdexcept>

#include "semigroup.func.hpp"
#include "monoid.func.hpp"
#include "functor.func.hpp"
#include "traversable.func.hpp"
#include "../Control/applicative.func.hpp"
#include "../Control/monad.func.hpp"
#include "../Basic/eq.func.hpp"
#include "../Basic/ord.func.hpp"
#include "../IO/show.func.hpp"

#ifndef MAYBE_FUNC_HPP
#define MAYBE_FUNC_HPP

// The `Maybe` type encapsulates an optional value. A value of type `Maybe` `a` either contains a value of type `a`
// (represented as `Just` `a`), or it is empty (represented as `Nothing`). Using `Maybe` is a good way to deal with errors or exceptional cases
// without resorting to drastic measures
//
// The `Maybe` type is also a monad. It is a simple kind of error monad, where all errors are represented by Nothing. 
// A richer error monad can be built using the `Either` type. 
template<typename T>
struct Maybe {
    enum { Nothing, Just } maybeVal;
    T justVal = justVal;
};

template<typename T>
inline const Maybe<T> nothing = {Maybe<T>::Nothing};

template<typename T>
inline Maybe<T> just(T x) { return {Maybe<T>::Just, x}; }

template<typename T>
inline const Maybe<T> _mempty<Maybe<T>> = nothing<T>;

template<typename T>
Maybe<T> sappend_c(std::function<T(T,T)> sappendF, Maybe<T> m, Maybe<T> ms) {
    switch(ms.maybeVal) {
        case (Maybe<T>::Nothing): return m;
        case (Maybe<T>::Just): {
            if (m.maybeVal == Maybe<T>::Just) {
                ms.justVal = sappendF(m.justVal, ms.justVal);
                return ms;
            }
            return ms;
        }                    
    }
    throw std::invalid_argument("maybeVal not in Nothing, Just");
}

template<typename Ta, typename Tb>
Maybe<Tb> fmap_c(std::function<Tb(Ta)> f, Maybe<Ta> x) {
    switch (x.maybeVal) {
        case (Maybe<Ta>::Just): return just(f(x.justVal));
        case (Maybe<Ta>::Nothing): return nothing<Tb>;
    }
    throw std::invalid_argument("maybeVal not in Nothing, Just");
}

template<template<typename ...> class F, typename T1, typename T2>
F<Maybe<T2>> traverse_c(std::function<F<Maybe<T2>>(Maybe<T2>)> pureF, 
                    std::function<F<Maybe<T2>>(std::function<Maybe<T2>(T2)>, F<T2>)>, 
                    std::function<F<Maybe<T2>>(std::function<Maybe<T2>(T2, Maybe<T2>)>, F<T2>, F<Maybe<T2>>)>, 
                    std::function<F<T2>(T1)> f, Maybe<T1> x) {
    return x.maybeVal == Maybe<T1>::Nothing ? pureF(nothing<T2>):fmapF(just<T2>, f(x.justVal));
}

template<typename T> 
inline Maybe<T> pure_c(Maybe<T>*, T x) { return just(x); }

template<typename Ta, typename Tb> 
Maybe<Tb> ap_c(Maybe<std::function<Tb(Ta)>> f, Maybe<Ta> x) {
    switch (f.maybeVal) {
        case (Maybe<std::function<Tb(Ta)>>::Just): return fmap(f.justVal, x);
        case (Maybe<std::function<Tb(Ta)>>::Nothing): return nothing<Tb>;
    }
    throw std::invalid_argument("maybeVal not in Nothing, Just");
}

template<typename Ta, typename Tb> 
Maybe<Tb> lazyAp_c(Maybe<std::function<Tb(Ta)>> f, std::function<Maybe<Ta>()> x) {
    switch (f.maybeVal) {
        case (Maybe<std::function<Tb(Ta)>>::Just): return fmap(f.justVal, x());
        case (Maybe<std::function<Tb(Ta)>>::Nothing): return nothing<Tb>;
    }
    throw std::invalid_argument("maybeVal not in Nothing, Just");
}

template<typename T>
inline const Maybe<T> _emptyA<Maybe<T>> = nothing<T>;

template<typename T>
Maybe<T> aplus_c(Maybe<T> x, const Maybe<T>& xs) {
    switch(x.maybeVal) {
        case (Maybe<T>::Just): return x;
        case (Maybe<T>::Nothing): return xs;
    }
    throw std::invalid_argument("maybeVal not in Nothing, Just");
}

template<typename Ta, typename Tb>
Maybe<Tb> bind_c(Maybe<Ta> x, std::function<Maybe<Tb>(Ta)> f) {
    switch (x.maybeVal) {
        case (Maybe<Ta>::Just): return f(x.justVal);
        case (Maybe<Ta>::Nothing): return nothing<Tb>;
    }
    throw std::invalid_argument("maybeVal not in Nothing, Just");
}

template<typename T>
bool eq_c(std::function<bool(T,T)> eqF, Maybe<T> a, Maybe<T> b) {
    switch (b.maybeVal) {
        case Maybe<T>::Just: return a.maybeVal==Maybe<T>::Just ? eqF(a.justVal, b.justVal):false;
        case Maybe<T>::Nothing: return a.maybeVal==Maybe<T>::Nothing;
    }
    throw std::invalid_argument("maybeVal not in Just, Nothing");
};

template<typename T>
bool lte_c(std::function<bool(T,T)> lteF, Maybe<T> a, Maybe<T> b) {
    switch (b.maybeVal) {
        case Maybe<T>::Just: return a.maybeVal==Maybe<T>::Just ? lteF(a.justVal, b.justVal):false;
        case Maybe<T>::Nothing: return a.maybeVal==Maybe<T>::Nothing;
    }
    throw std::invalid_argument("maybeVal not in Just, Nothing");
};

template<typename T, typename Targ>
requires (std::same_as<Targ, T> || std::same_as<Targ, const T&>) 
    std::string show_c(std::function<std::string(Targ)> showF, Maybe<T> x) {
        switch (x.maybeVal) {
            case Maybe<T>::Nothing: return "Nothing";
            case Maybe<T>::Just: return "Just " + showF(x.justVal);
        }
        throw std::invalid_argument("maybeVal not in Nothing, Just");
    }

#endif
