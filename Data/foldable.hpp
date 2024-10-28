#include "foldable.func.hpp"
#include "foldable.conc.hpp" 

#include "monoid.hpp"
#include "maybe.hpp"
#include "../Control/monad.hpp"
#include "../Basic/eq.hpp"
#include "../Basic/ord.hpp"
#include "list.hpp"
#include "../IO/IO.func.hpp"
#include <functional>

#ifndef FOLDABLE_HPP
#define FOLDABLE_HPP

template<template<typename ...> class T, typename Ta, typename Tb>
requires (Foldable<T, Ta>) inline Tb foldr(std::function<Tb(Ta, Tb)> f, Tb z, T<Ta> t) { return foldr_c(f, z, t); };

template<template<typename, typename> class T, typename Ta, typename T1, typename T2>
requires (Foldable2<T, Ta, T1>) inline T2 foldr(std::function<T2(T1, T2)> f, T2 z, T<Ta, T1> t) { return foldr_c2(f, z, t); };

// Left-associative fold of a structure. Should be specialized for efficiency 
template<template<typename...> class T, typename Ta, typename Tb>
Tb foldl(std::function<Tb(Tb,Ta)> f, Tb z, T<Ta> as) {
    std::function<Tb(Ta, std::function<Tb(Tb)>, Tb)> f2 = [&f] (Ta a, std::function<Tb(Tb)> g, Tb x) -> Tb {
            return g(f(x, a));    
        };
    std::function<std::function<Tb(Tb)>(Ta, std::function<Tb(Tb)>)> f3 = [&f2] (Ta a, std::function<Tb(Tb)> g) -> std::function<Tb(Tb)> {
            return std::bind_front(f2, a, g);
        };
    return foldr(f3, id<Tb>, as)(z);
}

template<template<typename, typename> class T, typename Ta, typename T1, typename T2>
T2 foldl(std::function<T2(T2,T1)> f, T2 z, T<Ta, T1> as) {
    std::function<T2(T1, std::function<T2(T2)>, T2)> f2 = [&f] (T1 a, std::function<T2(T2)> g, T2 x) -> T2 {
            return g(f(x, a));    
        };
    std::function<std::function<T2(T2)>(T1, std::function<T2(T2)>)> f3 = [&f2] (T1 a, std::function<T2(T2)> g) -> std::function<T2(T2)> {
            return std::bind_front(f2, a, g);
        };
    return foldr(f3, id<T2>, as)(z);
}

// Map each element of the structure into a monoid, and combine the results. This fold is right-associative.
template<template<typename...> class T, typename M, typename Ta>
M foldMap(std::function<M(Ta)> f, T<Ta> as) {
    M m = mempty<M>;
    std::function<M(Ta, M)> f2 = [&f] (Ta a, M ms) -> M {
            return mappend(f(a), ms);
        };
    return foldr(f2, m, as);
}

template<template<typename, typename> class T, typename M, typename Ta, typename T1>
M foldMap(std::function<M(T1)> f, T<Ta, T1> as) {
    M m = mempty<M>;
    std::function<M(T1, M)> f2 = [&f] (T1 a, M ms) -> M {
            return mappend(f(a), ms);
        };
    return foldr(f2, m, as);
}

template<template<typename ...> class T, typename M> 
inline M fold(T<M> x) { return foldMap(id<M>, x); }

template<template<typename ...> class T, typename Ta>
Ta foldr1(std::function<Ta(Ta,Ta)> f, T<Ta> xs) {
    std::function<Maybe<Ta>(Ta, Maybe<Ta>)> mf = [&f] (Ta x, Maybe<Ta> m) { 
        return just(maybe(x, partial(f, x), m)); };
    Maybe<Ta> res = foldr(mf, nothing<Ta>, xs);
    if (res.maybeVal == Maybe<Ta>::Nothing)
        throw std::invalid_argument("Empty structure to foldr1");
    return res.justVal;
}

// No std::bind_back in Clang 18.1.8's C++23, so extra lambda 
template<template<typename ...> class T, typename Ta>
Ta foldl1(std::function<Ta(Ta,Ta)> f, T<Ta> xs) {
    std::function<Maybe<Ta>(Ta, Maybe<Ta>)> mf = [&f] (Maybe<Ta> m, Ta y) {
        return just(maybe(y, partial(flip(f), y), m));
    };
    Maybe<Ta> res = foldl(mf, nothing<Ta>, xs); 
    if (res.maybeVal == Maybe<Ta>::Nothing)
        throw std::invalid_argument("Empty structure to foldl1");
    return res.justVal;
}

// Mutating foldr, not in functional land anymore
// Overload this for specific operations
template<template<typename ...> class T, class C, typename Ta, typename Tb>
requires (!std::ranges::bidirectional_range<T<Ta>>
      &&   std::invocable<C, Ta, Tb&>)
inline void foldrIP(C f, Tb& y, T<Ta> xs) {
    std::function<Tb*(Ta, Tb*)> innerF = [f] (Ta x, Tb* y1) {
        f(x, *y1);
        return y1;
    };
    foldr(innerF, &y, xs);
}

template<template<typename ...> class T, class C, typename Ta, typename Tb>
requires (std::ranges::bidirectional_range<T<Ta>> 
      && std::invocable<C, Ta, Tb&>) 
inline void foldrIP(C f, Tb& z, const T<Ta>& as) {
    std::ranges::reverse_view rvas{as};
    for (auto a : rvas) {
        f(a, z);
    }
}

template<template<typename ...> class T, typename Ta>
inline std::list<Ta> toList(T<Ta> t) { return foldr(asF(cons<Ta>), std::list<Ta>{}, t); }

template<template<typename ...> class T, typename Ta>
inline bool nullT(T<Ta> t) { return foldr(asF([] (Ta, bool) {return false;}), true, t); }

template<template<typename ...> class T, typename Ta>
requires (!std::ranges::sized_range<T<Ta>> && Foldable<T, Ta>)
    inline size_t length(T<Ta> t) { return foldl(asF([] (Ta, size_t i) {return i + 1;}), 0, t); }

template<template<typename ...> class T, typename Ta>
inline bool any(std::function<bool(Ta)> f, T<Ta> xs) { return foldr(asF([&f,xs] (Ta x, bool b) {return b || f(x);}), false, xs); }

template<template<typename ...> class T, typename Ta> 
inline bool all(std::function<bool(Ta)> f, T<Ta> xs) { return foldr(asF([&f,xs] (Ta x, bool b) {return b && f(x);}), true, xs); }

template<template<typename ...> class T, typename Ta>
inline bool elem(Ta x, T<Ta> xs) { return any(asF<bool>(std::bind_front(static_cast<bool(*)(Ta,Ta)>(eq), x), (Ta*)nullptr), xs); }

template<template<typename ...> class T, typename Ta>
requires (Ord<Ta>) Ta maximum(T<Ta> ls) { return foldr1(asF(max<Ta>), ls);}

template<template<typename ...> class T, typename Ta> 
requires (Ord<Ta>) Ta minimum(T<Ta> ls) { return foldr1(asF(min<Ta>), ls); }

template<template<typename ...> class T, typename Ta>
inline Ta sum(T<Ta> xs) { return foldr(asF(Ta::operator+), fromInteger<Ta>(0), xs); } 

template<template<typename ...> class T, typename Ta>
inline Ta product(T<Ta> xs) { return foldr(asF(Ta::operator*), fromInteger<Ta>(1), xs); }

template<template<typename ...> class T, template<typename ...> class M, typename Ta, typename Tb>
M<Tb> foldrM(std::function<M<Tb>(Ta, Tb)> f, Tb z0, T<Ta> xs) {
    std::function<M<Tb>(std::function<M<Tb>(Tb)>, Ta, Tb)> c = [&f] (std::function<M<Tb>(Tb)> k, Ta x, Tb z) { 
        return bind(f(x, z), k);
    };
    std::function<std::function<M<Tb>(Tb)>(std::function<M<Tb>(Tb)>, Ta)> cF = [&c] (std::function<M<Tb>(Tb)> k, Ta x) {
        return asF<M<Tb>>(std::bind_front(c, k, x), (Tb*)nullptr);
    };
    return foldl(cF, asF(returnM<M, Tb>), xs)(z0);
}

template<template<typename ...> class T, template<typename ...> class M, typename Ta, typename Tb>
M<Tb> foldlM(std::function<M<Tb>(Tb, Ta)> f, Tb z0, T<Ta> xs) {
    std::function<M<Tb>(Ta, std::function<M<Tb>(Tb)>, Tb)> c = [&f] (Ta x, std::function<M<Tb>(Tb)> k, Tb z) { 
        return bind(f(z, x), k);
    };
    std::function<std::function<M<Tb>(Tb)>(Ta, std::function<M<Tb>(Tb)>)> cF = [&c] (Ta x, std::function<M<Tb>(Tb)> k) {
        return asF<M<Tb>>(std::bind_front(c, x, k), (Tb*)nullptr);
    };
    return foldr(cF, asF(returnM<M, Tb>), xs)(z0);
}

// Left associative since foldr and ignoreA/(*>) are strict in all args 
template<template<typename ...> class T, template<typename ...> class F, typename Ta, typename Tb>
inline F<void*> traverse_(std::function<F<Tb>(Ta)> f, T<Ta> t) {
    return foldr(asF<F<void*>, Ta, F<void*>>([&f] (Ta x, F<void*> k) {return ignoreA(f(x), k);}), 
                 pure<F>((void*)nullptr), t);
}

template<template<typename ...> class T, template<typename ...> class F, typename Ta, typename Tb>
inline F<void*> for_(T<Ta> t, std::function<F<Tb>(Ta)> f) { return traverse_(f, t); }

template<template<typename ...> class T, template<typename ...> class M, typename Ta, typename Tb>
inline M<void*> mapM_(std::function<M<Tb>(Ta)> f, const T<Ta>& t) {
    return foldr(asF<M<void*>, Ta, M<void*>>([&f] (Ta x, M<void*> k) {return constbind(f(x), k);}), returnM<M>((void*)nullptr), t);
}

template<template<typename ...> class T, typename Ta, typename Tb, typename Targ>
requires (std::same_as<Ta, Targ> || std::same_as<const Ta&, Targ>)
    inline IO<void*> mapM_(std::function<IO<Tb>(Targ)> f, const T<Ta>& t) {
        IO<void*> res = returnM<IO>((void*)nullptr); 
        foldrIP([f] (Targ x, IO<void*>& k) {k.fuse(f(x));}, res, t);
        return res;
    }

template<template<typename ...> class T, template<typename ...> class M, typename Ta, typename Tb>
inline M<void*> forM_(T<Ta> t, std::function<M<Tb>(Ta)> f) { return mapM_(f, t); }

template<template<typename ...> class T, template<typename ...> class F, typename Ta>
inline F<void*> sequenceA_(T<F<Ta>> t) { return foldr(asF(ignoreA<F, Ta, void*>), pure<F>((void*)nullptr), t); }

template<template<typename ...> class T, template<typename ...> class M, typename Ta>
inline M<void*> sequence_(T<M<Ta>> t) { return foldr(asF(constbind<M, Ta, void*>), returnM<M>((void*)nullptr), t); }

template<template<typename ...> class T, template<typename ...> class L, typename Ta>
inline L<Ta> concat(T<L<Ta>> xs) {
    std::function<L<Ta>(L<Ta>, L<Ta>)> f2 = [] (L<Ta> x, L<Ta> y) {
        return foldr(asF(cons<L, Ta>), y, x); 
    };
    return foldr(f2, L<Ta>(), xs);
}

template<template<typename ...> class T, template<typename ...> class L, typename Ta, typename Tb>
inline L<Tb> concatMap(std::function<L<Tb>(Ta)> f, T<Ta> xs) {
    std::function<L<Tb>(Ta, L<Tb>)> f2 = [f] (Ta x, L<Tb> b) {
        return foldr(asF(cons<L, Ta>), b, f(x)); 
    };
    return foldr(f2, L<Tb>(), xs);
}

template<template<typename ...> class T>
inline bool andT(T<bool> bs) { return foldr(std::function<bool(bool,bool)>(std::logical_and()), true, bs); }

template<template<typename ...> class T>
inline bool orT(T<bool> bs) { return foldr(std::function<bool(bool,bool)>(std::logical_or()), false, bs); }

template<template<typename ...> class T, typename Ta>
inline bool notElem(Ta x, T<Ta> xs) { return !elem(x, xs); }

template<template<typename ...> class T, typename Ta>
inline Maybe<Ta> find(std::function<bool(Ta)> f, T<Ta> xs) {
    return foldl(asF<Maybe<Ta>, Maybe<Ta>, Ta>([&f] (Maybe<Ta> mx, Ta x) { return isJust(mx) ? mx:(f(x) ? just(x):nothing<Ta>); }), 
                 nothing<Ta>, xs);
}

#endif
