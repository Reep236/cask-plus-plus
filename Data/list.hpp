#include <cassert>
#include <ranges>
#include <functional>

#include "tuple.func.hpp"
#include "list.func.hpp"
#include "list.conc.hpp"
#include "../IO/IO.hpp"

#ifndef LIST_HPP
#define LIST_HPP

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) inline void prepend(T x, L<T>& xs) { prepend_f(x, xs); }

template<template<typename ...> class L, typename T>
requires (!ListLike<L, T> && Sequence<L, T>) inline void prepend(T x, L<T>& xs) { 
    prepend_t(x, xs); 
}

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) [[nodiscard("Use `prepend`")]] inline L<T> cons(T x, L<T> xs) { return prepend_f(x, xs); }

template<template<typename ...> class L, typename T>
requires (!ListLike<L, T> && Sequence<L, T>) [[nodiscard("Use `prepend`")]] inline L<T> cons(T x, L<T> xs) { 
    return prepend_t(x, xs); 
}

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) inline T head(const L<T>& xs) { return head_f(xs); }

template<template<typename ...> class L, typename T>
requires (!ListLike<L, T> && Sequence<L, T>) inline T head(const L<T>& xs) { return head_t(xs); }

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) inline void tailIP(L<T>& xs) { tail_f(xs); }

template<template<typename ...> class L, typename T>
requires (!ListLike<L, T> && Sequence<L, T>) inline void tailIP(L<T>& xs) { tail_t(xs); }

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) [[nodiscard("Use `tailIP`")]] L<T> tail(L<T> xs) { return tail_f(xs); }

template<template<typename ...> class L, typename T>
requires (!ListLike<L, T> && Sequence<L, T>) [[nodiscard("Use `tailIP`")]] L<T> tail(L<T> xs) { return tail_t(xs); }

template<template<typename ...> class L, typename T>
requires (ListLike<L, T> || Sequence<L, T>) inline L<T> singleton(T x) { return static_cast<L<T>(*)(T)>(singleton_t)(x); }

template<template<typename ...> class L, typename T>
requires (ListLike<L, T> || Sequence<L, T>) inline bool null(L<T> xs) { return null_t(xs); }

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) inline void append(T x, L<T>& xs) { append_t(x, xs); }

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) [[nodiscard("Use `append`")]] inline L<T> snoc(T x, L<T> xs) { return append_t(x, xs); }

template<template<typename ...> class L, typename T> 
requires (Sequence<L, T>) inline T last(const L<T>& xs) { return last_t(xs); }

template<template<typename ...> class L, typename T> 
requires (Sequence<L, T>) inline void initIP(L<T>& xs) { init_t(xs); }

template<template<typename ...> class L, typename T> 
requires (Sequence<L, T>) [[nodiscard("Use `initIP`")]] inline L<T> init(L<T> xs) { return init_t(xs); }

template<template<typename ...> class L, typename T> 
requires (Sequence<L, T>) inline void catIP(L<T>& ys, const L<T>& xs) { cat_t(ys, xs); }

template<template<typename ...> class L, typename T> 
requires (Sequence<L, T>) [[nodiscard("Use `catIP`")]] inline L<T> cat(L<T> ys, const L<T>& xs) { return cat_t(ys, xs); }

template<template<typename ...> class L, typename T>
requires (Indexable<L, T>) inline T index(L<T> xs, size_t i) { return xs[i]; }

template<template<typename ...> class L, typename T>
T index(L<T> xs, size_t i) {
    for (size_t _ = 0; _ < i; ++_)
        tailIP(xs);
    return head(xs);
}

template<template<typename ...> class L, typename Ta, typename Tb>
L<Tb> map(std::function<Tb(Ta)> f, L<Ta> xs) { 
    L<Tb> ys;
    for (Ta x : xs) 
        append(f(x), ys);
    return ys;
}

template<template<typename ...> class L, typename T>
L<T> _filterConsume(std::function<bool(T)> f, L<T>& xs) {
    if (null(xs)) return L<T>();
    
    T x = head(xs);
    tailIP(xs);

    return f(x) ? cons(x, filter(f, xs)):filter(f, xs);
}

template<template<typename ...> class L, typename T>
L<T> filter(std::function<bool(T)> f, L<T> xs) {
    return _filterConsume(f, xs);
}

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) L<T> filter(std::function<bool(T)> f, L<T> xs) {
    L<T> ys;
    for (T x : xs)
        if (f(x))
            append(x, ys);
    return ys;
}

// Get other implementations for free
template<template<typename ...> class C, typename T>
requires (std::ranges::bidirectional_range<C<T>>) inline C<T> reverse(C<T> xs) {
    return C<T>(xs.rbegin(), xs.rend());
}

template<template<typename ...> class C, typename T>
inline C<T> reverse(C<T> xs) {
   C<T>(*rev)(C<T>, C<T>) = [&rev] (C<T> pull, C<T> put) -> C<T> { return null(pull) ? put:rev(tail(pull), cons(head(pull), put)); };
   return rev(xs, C<T>());
} 

template<template<typename ...> class L, typename T>
L<T> intersperse(T sep, L<T> xs) {
    L<T> ys = singleton<L>(head(xs));
    for (auto x : xs | std::ranges::views::drop(1)) {
        append(sep, ys);
        append(x, ys);
    }
    return ys; 

}

template<template<typename ...> class L, typename T>
L<T> intercalate(L<T> sep, L<L<T>> xss) {
    L<T> ys = head(xss);
    for (auto xs : xss | std::ranges::views::drop(1)) {
        catIP(ys, sep);
        catIP(ys, xs);
    }
    return ys;
}

template<template<typename ...> class L, typename T>
L<L<T>> transpose(L<L<T>> xss) {
    if (null(xss)) return L<L<T>>{};

    L<T> xs = head(xss);
    tailIP(xss);

    if (null(xs)) return transpose(xss);

    T x = head(xs);
    tailIP(xs);
    L<T> hds;
    L<L<T>> tls;

    for (auto xs1 : xss) {
        append(head(xs1), hds);
        append(tail(xs1), tls);
    }

    prepend(x, hds);
    prepend(xs, tls);

    return cons(hds, transpose(tls));
}

template<template<typename ...> class L, typename T>
L<L<T>> subsequences(L<T> xs) {
    if (null(xs)) return singleton<L>(L<T>());

    T x = last(xs);
    L<L<T>> yssRem = subsequences(init(xs)); 
    return cat(yssRem, map(partial(snoc<L, T>, x), yssRem));
}


// Should modify this to favor last/init over head/tail if it uses append anyways...
template<template<typename ...> class L, typename T>
L<L<T>> permutations(L<T> xs) {
    if (null(xs)) return singleton<L>(L<T>());

    L<L<T>> res;
    L<T> front;
    L<T> back = xs;

    T x = head(back);
    tailIP(back);
    
    for (; !null(back); x=head(back), tailIP(back)) { 
        L<L<T>> futureRes = permutations(cat(front, back));

        catIP(res, map(partial(cons<L, T>, x), futureRes));
        append(x, front);
    }
    
    catIP(res, map(partial(cons<L, T>, x), permutations(front)));

    return res;
}


template<template<typename ...> class L, typename Ta, typename Tb>
inline L<Tb> scanl(std::function<Tb(Tb, Ta)> f, Tb z, const L<Ta>& xs) {
    return null(xs) ? singleton<L>(z)
                    : cons(z, scanl(f, f(z, head(xs)), tail(xs)));
}

template<template<typename ...> class L, typename Ta, typename Tb> 
requires (Sequence<L, Ta>) L<Tb> scanl(std::function<Tb(Tb, Ta)> f, Tb z, const L<Ta>& xs) {
    L<Tb> res = singleton<L>(z);
    for (Ta x : xs) {
        z = f(z, x);
        append(z, res);
    }
    return res;
}

template<template<typename ...> class L, typename T>
inline L<T> scanl1(std::function<T(T, T)> f, const L<T>& xs) {
    return null(xs) ? L<T>()
                    : scanl(f, head(xs), tail(xs));
}

template<template<typename ...> class L, typename Ta, typename Tb>
L<Tb> scanr(std::function<Tb(Ta, Tb)> f, Tb z, const L<Ta>& xs) {
    if (null(xs)) return singleton<L>(z);
    
    L<Tb> futureRes = scanr(f, z, tail(xs));
    return cons(f(head(xs), head(futureRes)), futureRes);
}

template<template<typename ...> class L, typename Ta, typename Tb>
requires (Sequence<L, Ta>) L<Tb> scanr(std::function<Tb(Ta, Tb)> f, Tb z, const L<Ta>& xs) {
    L<Tb> res = singleton<L>(z);

    for (Ta x : reverse(xs)) {
        z = f(x, z);
        prepend(z, res);
    }

    return res;
}

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) inline L<T> scanr1(std::function<T(T, T)> f, const L<T>& xs) {
    return scanr(f, last(xs), init(xs));
}

template<template<typename ...> class L, typename T>
L<T> scanr1(std::function<T(T, T)> f, const L<T>& xs) {
    if (null(xs)) return L<T>();

    if (null(tail(xs))) return xs;

    L<T> futureRes = scanr1(f, tail(xs));
    
    return cons(f(head(xs), head(futureRes)), futureRes);
}

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) inline L<T> replicate(int n, T x) {
    L<T> res;
    for (int i = 0; i < n; ++i)
        prepend(x, res);
    return res;
}

template<template<typename ...> class L, typename T>
inline L<T> replicate(int n, T x) {
    L<T> res;
    for (int i = 0; i < n; ++i)
        append(x, res);
    return res;
}

template<template<typename ...> class L, typename T>
requires(ListLike<L, T>) inline L<T> take(int n, L<T> xs) {
    return (n == 0 || null(xs)) ? L<T>():cons(head(xs), take(n - 1, tail(xs)));
}

template<template<typename ...> class L, typename T>
L<T> take(int n, L<T> xs) {
    L<T> res;
    for (int i = 0; i < n && !null(xs); ++i) {
        append(head(xs));
        tailIP(xs);   
    }
    return res;
}

template<template<typename ...> class L, typename T>
inline L<T> drop(int n, L<T> xs) {
    for (int i = 0; i < n && !null(xs); ++i)
        tailIP(xs);
    return xs;
}

template<template<typename ...> class L, typename T>
L<T> takeWhile(std::function<bool(T)> f, L<T> xs) {
    L<T> res;
    while(!null(xs) && f(head(xs))) {
       append(head(xs), res);
       tailIP(xs);
    }
    return res;
}

template<template<typename ...> class L, typename T>
requires (ListLike<L, T>) L<T> takeWhile(std::function<bool(T)> f, L<T> xs) {
    return (null(xs) || !f(head(xs))) ? L<T>():cons(head(xs), takeWhile(f, tail(xs)));
}

template<template<typename ...> class L, typename T>
inline L<T> dropWhile(std::function<bool(T)> f, L<T> xs) {
    while(!null(xs) && f(head(xs)))
        tailIP(xs);
    return xs;
}

template<template<typename ...> class L, typename T>
std::pair<L<T>, L<T>> span(std::function<bool(T)> f, L<T> xs) {
    if (null(xs)) return std::pair(xs, xs);
    T x = head(xs);
    if (f(x)) {
        std::pair<L<T>, L<T>> futureRes = span(f, tail(xs));
        return std::pair(cons(x, fst(futureRes)), snd(futureRes));
    }
    return std::pair(L<T>(), xs);
}

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) std::pair<L<T>, L<T>> span(std::function<bool(T)> f, L<T> xs) {
    if (null(xs)) return std::pair(xs, xs);
    L<T> front;
    L<T> back = xs;
    
    for (T x = head(back); !null(back) && f(x); x=head(back)) {
        append(x, front);
        tailIP(back);
    }

    return std::pair(front, back);
}

template<template<typename ...> class L, typename T>
std::pair<L<T>, L<T>> breakL(std::function<bool(T)> f, L<T> xs) {
    if (null(xs)) return std::pair(xs, xs);
    T x = head(xs);

    if (f(x) || null(xs)) 
        return std::pair(L<T>(), xs);
    
    std::pair<L<T>, L<T>> futureRes = breakL(f, tail(xs));
    return std::pair(cons(x, fst(futureRes)), snd(futureRes));
}

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) std::pair<L<T>, L<T>> breakL(std::function<bool(T)> f, L<T> xs) {
    if (null(xs)) return std::pair(xs, xs);
    L<T> front;
    L<T> back = xs;
    
    for (T x = head(back); !null(back) && !f(x); x = head(back)) {
        append(x, front);
        tailIP(back);
    }

    return std::pair(front, back);
}

template<template<typename ...> class L, typename T>
std::pair<L<T>, L<T>> splitAt(int prefixLength, const L<T>& xs) {
    if (prefixLength <= 0 || null(xs)) return std::pair(L<T>(), xs);
    
    auto futureRes = splitAt(prefixLength - 1, tail(xs) );
    return std::pair(cons(head(xs), std::get<0>(futureRes)), std::get<1>(futureRes));
}

template<template<typename ...> class L, typename T>
requires (Sequence<L, T>) std::pair<L<T>, L<T>> splitAt(int prefixLength, L<T> xs) {
    L<T> front;
    L<T> back = xs;

    for (int i = 0; i < prefixLength; ++i) {
        append(head(back), front);
        tailIP(back);
    }

    return std::pair(front, back);
}

template<template<typename ...> class L, typename Ta, typename Tb>
Maybe<Tb> lookup(Ta key, L<std::pair<Ta, Tb>> assocs) {
    for (auto assoc : assocs)
        if (std::get<0>(assoc) == key)
            return just(std::get<1>(assoc));
    return nothing<Tb>;
}  

template<template<typename ...> class L, typename Ta, typename Tb>
L<std::pair<Ta, Tb>> _zipConsume(L<Ta>& xs, L<Tb>& ys) {
    if (null(xs) || null(ys)) return L<std::pair<Ta, Tb>>();
    Ta x = head(xs);
    Tb y = head(ys);
    tailIP(xs);
    tailIP(ys);
    return cons(std::pair(x, y), _zipConsume(xs, ys));
}

template<template<typename ...> class L, typename Ta, typename Tb>
L<std::pair<Ta, Tb>> zip(L<Ta> xs, L<Tb> ys) {
    return _zipConsume(xs, ys);
}

template<template<typename ...> class L, typename Ta, typename Tb>
requires (Sequence<L, Ta>) L<std::pair<Ta, Tb>> zip(L<Ta> xs, L<Tb> ys) {
    L<std::pair<Ta, Tb>> res;
    for (Ta x : xs) {
        auto y = head(ys);
        tailIP(ys);
        append(std::pair(x, y), res);
    }
    return res;
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
L<std::tuple<Ta, Tb, Tc>> _zip3Consume(L<Ta>& xs, L<Tb>& ys, L<Tc>& zs) {
    if (null(xs) || null(ys) || null(zs)) return L<std::tuple<Ta, Tb, Tc>>();
    Ta x = head(xs);
    Tb y = head(ys);
    Tc z = head(zs);
    tailIP(xs);
    tailIP(ys);
    tailIP(zs);
    return cons(std::tuple(x, y, z), _zip3Consume(xs, ys, zs));
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
L<std::tuple<Ta, Tb, Tc>> zip3(L<Ta> xs, L<Tb> ys, L<Tc> zs) {
    return _zip3Consume(xs, ys, zs);
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
requires(Sequence<L, Ta>) L<std::tuple<Ta, Tb, Tc>> zip3(L<Ta> xs, L<Tb> ys, L<Tc> zs) {
    L<std::tuple<Ta, Tb, Tc>> res;
    for (Ta x : xs) {
        auto y = head(ys);
        tailIP(ys);
        auto z = head(zs);
        tailIP(zs);
        append(std::tuple(x, y, z), res);
    }
    return res;
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
L<Tc> _zipWithConsume(std::function<Tc(Ta, Tb)> f, L<Ta>& xs, L<Tb>& ys) {
    if (null(xs) || null(ys)) return L<Tc>();
    Ta x = head(xs);
    tailIP(xs);
    Tb y = head(ys);
    tailIP(ys);
    return cons(f(x, y), _zipWithConsume(f, xs, ys));
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
L<Tc> zipWith(std::function<Tc(Ta, Tb)> f, L<Ta> xs, L<Tb> ys) {
    return _zipWithConsume(f, xs, ys);
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
requires(Sequence<L, Ta>) L<Tc> zipWith(std::function<Tc(Ta, Tb)> f, L<Ta> xs, L<Tb> ys) {
    L<Tc> res; 
    for (Ta x : xs) {
        auto y = head(ys);
        tailIP(ys);
        append(f(x, y), res);
    }
    return res;
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc, typename Td>
L<Td> _zipWith3Consume(std::function<Td(Ta, Tb, Tc)> f, L<Ta>& xs, L<Tb>& ys, L<Tc>& zs) {
    if (null(xs) || null(ys) || null(zs)) return L<Td>();
    Ta x = head(xs);
    tailIP(xs);
    Tb y = head(ys);
    tailIP(ys);
    Tb z = head(zs);
    tailIP(zs);
    return cons(f(x, y, z), _zipWith3Consume(f, xs, ys, zs));
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc, typename Td>
L<Td> zipWith3(std::function<Td(Ta, Tb, Tc)> f, L<Ta> xs, L<Tb> ys, L<Tc> zs) {
    return _zipWith3Consume(f, xs, ys, zs);
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc, typename Td>
requires(Sequence<L, Ta>) L<Td> zipWith3(std::function<Td(Ta, Tb, Tc)> f, L<Ta> xs, L<Tb> ys, L<Tc> zs ) {
    L<Td> res; 
    for (Ta x : xs) {
        auto y = head(ys);
        tailIP(ys);
        auto z = head(zs);
        tailIP(zs);
        append(f(x, y, z), res);
    }
    return res;
}

template<template<typename ...> class L, typename Ta, typename Tb>
std::pair<L<Ta>, L<Tb>> unzip(L<std::pair<Ta, Tb>> xys) {
    if (null(xys)) return std::pair(L<Ta>(), L<Tb>());
    
    std::pair<Ta, Tb> xy = head(xys);
    tailIP(xys);
    std::pair<L<Ta>, L<Tb>> futureRes = unzip(xys);
    return std::pair(cons(fst(xy), fst(futureRes)), cons(snd(xy), snd(futureRes)));
}

template<template<typename ...> class L, typename Ta, typename Tb>
requires (Sequence<L, Ta>) std::pair<L<Ta>, L<Tb>> unzip(L<std::pair<Ta, Tb>> xys) {
    L<Ta> xs;
    L<Tb> ys;

    for (auto xy : xys) {
        append(fst(xy), xs);
        append(snd(xy), ys);
    }

    return std::pair(xs, ys);
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
std::tuple<L<Ta>, L<Tb>, L<Tc>> unzip3(L<std::tuple<Ta, Tb, Tc>> xyzs) {
    if (null(xyzs)) return std::tuple(L<Ta>(), L<Tb>(), L<Tc>());
    
    std::tuple<Ta, Tb, Tc> xyz = head(xyzs);
    tailIP(xyzs);
    std::tuple<L<Ta>, L<Tb>, L<Tc>> futureRes = unzip3(xyzs);
    return std::tuple(cons(std::get<0>(xyz), std::get<0>(futureRes)), cons(std::get<1>(xyz), std::get<1>(futureRes), 
                      cons(std::get<2>(xyz), std::get<2>(futureRes))));
}

template<template<typename ...> class L, typename Ta, typename Tb, typename Tc>
requires (Sequence<L, Ta>) std::tuple<L<Ta>, L<Tb>, L<Tc>> unzip3(L<std::tuple<Ta, Tb, Tc>> xyzs) {
    L<Ta> xs;
    L<Tb> ys;
    L<Tc> zs;

    for (auto xyz : xyzs) {
        append(std::get<0>(xyz), xs);
        append(std::get<1>(xyz), ys);
        append(std::get<2>(xyz), zs);
    }

    return std::tuple(xs, ys, zs);
}

//TODO: Ensure this works with "\r\n"
template<template<typename ...> class L>
L<std::string> lines(std::string s) {
    L<std::string> res;
    size_t hold = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        if (s.at(i) != '\n')
            continue;
        append(s.substr(hold, i - hold), res);
        hold = i; 
    }
    if (hold < s.size() - 1)
        append(s.substr(hold, s.size() - hold), res);
    return res;
}

template<template<typename ...> class L>
requires (ListLike<L, std::string>) L<std::string> lines(std::string s) {
    L<std::string> res;
    size_t hold = s.size() - 1;
    for (size_t i = s.size() - 2; i >= 0; --i) {
        if (s.at(i) != '\n')
            continue;
        prepend(s.substr(i, hold - i), res);
        hold = i;
    }

    if (hold > 0)
        prepend(s.substr(0, hold), res);

    return res;
}

template<template<typename ...> class L>
L<std::string> words(std::string s) {
    L<std::string> res;
    size_t hold = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        if (!std::isspace(s.at(i)))
            continue;
        append(s.substr(hold, i - hold), res);
        hold = i; 
    }
    if (hold < s.size() - 1)
        append(s.substr(hold, s.size() - hold), res);
    return res;
}

template<template<typename ...> class L>
requires (ListLike<L, std::string>) L<std::string> words(std::string s) {
    L<std::string> res;
    size_t hold = s.size() - 1;
    for (size_t i = s.size() - 2; i >= 0; --i) {
        if (!std::isspace(s.at(i)))
            continue;
        prepend(s.substr(i, hold - i), res);
        hold = i;
    }

    if (hold > 0)
        prepend(s.substr(0, hold), res);

    return res;
}

#endif
