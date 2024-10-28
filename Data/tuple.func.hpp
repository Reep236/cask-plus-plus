#include <functional>
#include <utility>

#ifndef TUPLE_FUNC_HPP
#define TUPLE_FUNC_HPP

template<typename Ta, typename Tb>
Ta fst(std::pair<Ta, Tb> p) { return std::get<0>(p); }

template<typename Ta, typename Tb>
Tb snd(std::pair<Ta, Tb> p) { return std::get<1>(p); }

template<typename Ta, typename Tb, typename Tc>
Tc curry(std::function<Tc(std::pair<Ta, Tb>)> f, Ta x, Tb y) { return f(std::pair<Ta,Tb>{x, y}); }

template<typename Ta, typename Tb, typename Tc>
Tc uncurry(std::function<Tc(Ta, Tb)> f, std::pair<Ta, Tb> p) { return f(std::get<0>(p), std::get<1>(p)); }

template<typename Ta, typename Tb>
std::pair<Tb, Ta> swap(std::pair<Ta, Tb> p) { return std::pair<Tb,Ta>{std::get<1>(p), std::get<0>(p)}; }

#endif
