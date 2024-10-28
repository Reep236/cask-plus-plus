#include <stdexcept>
#include <variant>
#include "../Basic/eq.func.hpp"
#include "../Basic/ord.func.hpp"
#include "../IO/show.func.hpp"
#include "../IO/read.func.hpp"
#include "../Data/functor.func.hpp"
#include "../Data/foldable.func.hpp"
#include "../Data/traversable.func.hpp"
#include "../Control/applicative.func.hpp"
#include "../Control/monad.func.hpp"

#ifndef EITHER_FUNC_HPP
#define EITHER_FUNC_HPP

template<typename Ta, typename Tb> 
struct Either { 
    enum { Left, Right } eitherT;
    std::variant<Ta, Tb> eitherVal;
};

template<typename Ta, typename Tb>
Either<Ta, Tb> left(Ta x) { return {Either<Ta,Tb>::Left, x}; }

template<typename Ta, typename Tb>
Either<Ta, Tb> right(Tb x) { return {Either<Ta,Tb>::Right, x}; }

template<typename Ta, typename Tb>
bool eq_c2(std::function<bool(Ta,Ta)> eqF1, std::function<bool(Tb,Tb)> eqF2, Either<Ta, Tb> a, Either<Ta, Tb> b) {
    switch (a.eitherT) {
        case Either<Ta, Tb>::Left:  return b.eitherT == Either<Ta, Tb>::Left  ? eqF1(std::get<Ta>(a.eitherVal), std::get<Ta>(b.eitherVal)):false;
        case Either<Ta, Tb>::Right: return b.eitherT == Either<Ta, Tb>::Right ? eqF2(std::get<Tb>(a.eitherVal), std::get<Tb>(b.eitherVal)):false;
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
}

template<typename Ta, typename Tb>
bool lte_c2(std::function<bool(Ta,Ta)> lteF1, std::function<bool(Tb,Tb)> lteF2, Either<Ta, Tb> a, Either<Ta, Tb> b) {
    switch (a.eitherT) {
        case Either<Ta, Tb>::Left:  return b.eitherT == Either<Ta, Tb>::Left  ? lteF1(std::get<Ta>(a.eitherVal), std::get<Ta>(b.eitherVal)):true;
        case Either<Ta, Tb>::Right: return b.eitherT == Either<Ta, Tb>::Right ? lteF2(std::get<Tb>(a.eitherVal), std::get<Tb>(b.eitherVal)):false;
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
}

template<typename Ta, typename Tb>
std::string show_c2(std::function<std::string(Ta)> showF1, std::function<std::string(Tb)> showF2, Either<Ta, Tb> x) {
    switch (x.eitherT) {
        case Either<Ta, Tb>::Left:  return "Left " + showF1(std::get<Ta>(x.eitherVal));
        case Either<Ta, Tb>::Right: return "Right " + showF2(std::get<Tb>(x.eitherVal));
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
}

template<typename Ta, typename T1, typename T2> 
Either<Ta, T2> fmap_c2(std::function<T2(T1)> f, Either<Ta, T1> x) {
    return x.eitherT == Either<Ta, T1>::Left ? left<Ta, T2>(std::get<Ta>(x.eitherVal)):right<Ta, T2>(f(std::get<T1>(x.eitherVal)));
}

template<typename Ta, typename T1, typename T2>
T2 foldr_c2(std::function<T2 (T1, T2)> f, T2 z, Either<Ta, T1> t) {
    switch (t.eitherT) {
        case Either<Ta, T1>::Left:  return z;
        case Either<Ta, T1>::Right: return f(std::get<T1>(t.eitherVal), z);
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
};

template<template<typename ...> class F, typename Ta, typename T1, typename T2>
F<Either<Ta,T2>> traverse_c2(std::function<F<Either<Ta,T2>>(Either<Ta,T2>)> pureF, 
                    std::function<F<Either<Ta,T2>>(std::function<Either<Ta,T2>(T2)>, F<T2>)> fmapF, 
                    std::function<F<Either<Ta,T2>>(std::function<Either<Ta,T2>(T2, Either<Ta,T2>)>, F<T2>, F<Either<Ta,T2>>)>, 
                    std::function<F<T2>(T1)> f, Either<Ta,T1> xs) {
    switch (xs.eitherT) { 
        case Either<Ta,T2>::Left:  return pureF(left<Ta,T2>(std::get<Ta>(xs.eitherVal)));
        case Either<Ta,T2>::Right: return fmapF(asF(right<Ta, T2>), f(std::get<T1>(xs.eitherVal)));
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
}; 

template<typename Ta, typename T>
inline Either<Ta, T> pure_c2(Either<Ta, T>*, T x) { return right<Ta, T>(x); }

template<typename Ta, typename T1, typename T2>
Either<Ta, T2> ap_c2(Either<Ta, std::function<T2(T1)>> ef, Either<Ta, T1> ex) {
    switch (ef.eitherT) {
        case Either<Ta, std::function<T2(T1)>>::Left:  return left<Ta, T2>(std::get<Ta>(ef.eitherVal));
        case Either<Ta, std::function<T2(T1)>>::Right: return fmap_c2(std::get<std::function<T2(T1)>>(ef.eitherVal), ex);
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
}

template<typename Ta, typename T1, typename T2>
Either<Ta, T2> bind_c2(Either<Ta, T1> ex, std::function<Either<Ta, T2>(T1)> f) {
    switch (ex.eitherT) {
        case Either<Ta, T1>::Left:  return left<Ta, T2>(std::get<Ta>(ex.eitherVal));
        case Either<Ta, T1>::Right: return f(std::get<T1>(ex.eitherVal));
    }
    throw std::invalid_argument("IMPOSSIBLE: eitherT not in Left, Right");
}

#endif
