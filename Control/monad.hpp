#include "monad.func.hpp"
#include "monad.conc.hpp"

#ifndef MONAD_HPP
#define MONAD_HPP

template<template<typename ...> class M, typename T1, typename Tb, typename Targ>
requires (Monad<M, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Tb> bind(M<T1> a, std::function<M<Tb>(Targ)> f) { return bind_c(a, f); }

template<template<typename ...> class M, typename T1, typename Tb, typename Targ>
requires (Monad<M, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Tb> operator>>=(M<T1> a, std::function<M<Tb>(Targ)> f) { return bind_c(a, f); }

template<template<typename ...> class M, typename T1, typename Tb, typename Targ>
requires (Monad<M, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Tb> operator<<=(std::function<M<Tb>(Targ)> f, M<T1> a) { return bind_c(a, f); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2, typename Targ>
requires (Monad2<M, Ta, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Ta, T2> bind(M<Ta, T1> a, std::function<M<Ta, T2>(Targ)> f) { return bind_c2(a, f); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2, typename Targ>
requires (Monad2<M, Ta, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Ta, T2> operator>>=(M<Ta, T1> a, std::function<M<Ta, T2>(Targ)> f) { return bind_c2(a, f); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2, typename Targ>
requires (Monad2<M, Ta, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Ta, T2> operator<<=(std::function<M<Ta, T2>(Targ)> f, M<Ta, T1> a) { return bind_c2(a, f); }

template<template<typename ...> class M, typename T1, typename Tb, typename Targ>
requires (Monad<M, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Tb> bind(M<T1> a, M<Tb>(*f)(Targ)) { return bind_c(a, asF(f)); }

template<template<typename ...> class M, typename T1, typename Tb, typename Targ>
requires (Monad<M, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Tb> operator>>=(M<T1> a, M<Tb>(*f)(Targ)) { return bind_c(a, asF(f)); }

template<template<typename ...> class M, typename T1, typename Tb, typename Targ>
requires (Monad<M, T1> && (std::same_as<Targ, T1> || std::same_as<Targ, const T1&>)) 
    inline M<Tb> operator<<=(M<Tb>(*f)(Targ), M<T1> a) { return bind_c(a, asF(f)); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2>
requires (Monad2<M, Ta, T1>) inline M<Ta, T2> bind(M<Ta, T1> a, M<Ta, T2>(*f)(T1)) { return bind_c2(a, asF(f)); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2>
requires (Monad2<M, Ta, T1>) inline M<Ta, T2> operator>>=(M<Ta, T1> a, M<Ta, T2>(*f)(T1)) { return bind_c2(a, asF(f)); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2>
requires (Monad2<M, Ta, T1>) inline M<Ta, T2> operator<<=(M<Ta, T2>(*f)(T1), M<Ta, T1> a) { return bind_c2(a, asF(f)); }

template<template<typename ...> class M, typename Ta, typename Tb>
requires (Monad<M, Ta>) inline M<Tb> constbind(M<Ta> ma, M<Tb> mb) { return bind(ma, constf<Ta>(mb)); }

template<template<typename ...> class M, typename Ta, typename Tb>
requires (Monad<M, Ta>) inline M<Tb> operator>>(M<Ta> ma, M<Tb> mb) { return constbind(ma, mb); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2>
requires (Monad2<M, Ta, T1>) inline M<Ta, T2> constbind(M<Ta, T1> ma, M<Ta, T2> mb) { return bind(ma, constf<T1>(mb)); }

template<template<typename, typename> class M, typename Ta, typename T1, typename T2>
requires (Monad2<M, Ta, T1>) inline M<Ta, T2> operator>>(M<Ta, T1> ma, M<Ta, T2> mb) { return constbind(ma, mb); }

template<template<typename ...> class M, typename T>
requires (Monad<M, T>) inline M<T> returnM(T x) { return pure<M, T>(x); }

template<template<typename, typename> class M, typename Ta, typename T>
requires (Monad2<M, Ta, T>) inline M<Ta, T> returnM(T x) { return pure<M, Ta, T>(x); }

#endif
