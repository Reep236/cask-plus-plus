#include <functional>
#include <list>
#include "foldable.func.hpp"
#include "list.func.hpp"


#ifndef TRAVERSABLE_FUNC_HPP
#define TRAVERSABLE_FUNC_HPP


template<template<typename ...> class Tr, template<typename ...> class F, typename T1, typename T2>
F<Tr<T2>> traverse_c(std::function<F<Tr<T2>>(Tr<T2>)> pureF, 
                    std::function<F<Tr<T2>>(std::function<Tr<T2>(T2)>, F<T2>)> fmapF, 
                    std::function<F<Tr<T2>>(std::function<Tr<T2>(T2, Tr<T2>)>, F<T2>, F<Tr<T2>>)> liftA2F, 
                    std::function<F<T2>(T1)> f, Tr<T1> x) = delete; 

template<template<typename, typename> class Tr, template<typename ...> class F, typename Ta, typename T1, typename T2>
F<Tr<Ta,T2>> traverse_c2(std::function<F<Tr<Ta,T2>>(Tr<Ta,T2>)> pureF, 
                    std::function<F<Tr<Ta,T2>>(std::function<Tr<Ta,T2>(T2)>, F<T2>)> fmapF, 
                    std::function<F<Tr<Ta,T2>>(std::function<Tr<Ta,T2>(T2, Tr<Ta,T2>)>, F<T2>, F<Tr<Ta,T2>>)> liftA2F, 
                    std::function<F<T2>(T1)> f, Tr<Ta,T1> x) = delete; 


template<template<typename ...> class F, typename T1, typename T2>
F<std::list<T2>> traverse_c(std::function<F<std::list<T2>>(std::list<T2>)> pureF, 
                    std::function<F<std::list<T2>>(std::function<std::list<T2>(T2)>, F<T2>)>, 
                    std::function<F<std::list<T2>>(std::function<std::list<T2>(T2, std::list<T2>)>, F<T2>, F<std::list<T2>>)> liftA2F, 
                    std::function<F<T2>(T1)> f, std::list<T1> xs) {
    std::function<F<std::list<T2>>(T1, F<std::list<T2>>)> cons_f = [&f,&liftA2F] (T1 x, F<std::list<T2>> ys) {
        return liftA2F(asF(prepend_f<T2>), f(x), ys);   
    };
    std::list<T2> ys;
    return foldr_c(cons_f, pureF(ys), xs);
}; 

template<template<typename ...> class F, typename T1, typename T2>
F<std::vector<T2>> traverse_c(std::function<F<std::vector<T2>>(std::vector<T2>)> pureF, 
                    std::function<F<std::vector<T2>>(std::function<std::vector<T2>(T2)>, F<T2>)>, 
                    std::function<F<std::vector<T2>>(std::function<std::vector<T2>(T2, std::vector<T2>)>, F<T2>, F<std::vector<T2>>)> liftA2F, 
                    std::function<F<T2>(T1)> f, std::vector<T1> xs) {
    std::function<F<std::vector<T2>>(F<std::vector<T2>>, T1)> cons_f = [&f,&liftA2F] (F<std::vector<T2>> ys, T1 x) {
        return liftA2F(asF<std::vector<T2>, T2, std::vector<T2>>([] (T2 x, std::vector<T2> vs) {vs.push_back(x); return vs;}), f(x), ys);   
    };
    std::vector<T2> ys;
    return foldl(cons_f, pureF(ys), xs);
}; 

template<template<typename ...> class F, typename Ta, typename T1, typename T2>
F<std::pair<Ta,T2>> traverse_c2(std::function<F<std::pair<Ta,T2>>(std::pair<Ta,T2>)>, 
                    std::function<F<std::pair<Ta,T2>>(std::function<std::pair<Ta,T2>(T2)>, F<T2>)> fmapF, 
                    std::function<F<std::pair<Ta,T2>>(std::function<std::pair<Ta,T2>(T2, std::pair<Ta,T2>)>, F<T2>, F<std::pair<Ta,T2>>)>, 
                    std::function<F<T2>(T1)> f, std::pair<Ta,T1> xs) {
    std::function<std::pair<Ta, T2>(T2)> tplmkr = [xs] (T2 sd) { return std::pair<Ta, T2>{std::get<0>(xs), sd}; }  ;
    return fmapF(tplmkr, f(std::get<1>(xs)));
}; 

#endif
