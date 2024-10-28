#include "list.func.hpp"

#ifndef LIST_CONC_HPP
#define LIST_CONC_HPP

// X is ListLike if objects can be inserted and removed quickly from the front (emulating a singly linked list) 
template<template<typename...> class L, typename T>
concept ListLike = requires (L<T> xs) {
    { prepend_f(head_f(xs), xs) };
    { static_cast<L<T>(*)(T)>(singleton_t)(head_f(xs)) };
    { null_t(L<T>()) && null_t(tail_f(xs)) };
};


// X is a Sequence if objects can be inserted and removed at either end and Xs may be joined
// `std::list` is both ListLike and a Sequence
// `std::vector` is a Sequence but not ListLike 
// `std::stack` is ListLike but not a Sequence
template<template<typename...> class L, typename T>
concept Sequence = (ListLike<L, T> 
                  || requires (L<T> xs) {
                        { prepend_t(head_t(xs), xs) };
                        { static_cast<L<T>(*)(T)>(singleton_t)(head_t(xs)) };
                        { null_t(L<T>()) && null_t(tail_t(xs)) };
                     }) 
                  && requires (L<T> xs) {
                        { cat_t(xs, append_t(last_t(xs), xs)) };
                        { init_t(xs) };
                  };

template<template<typename ...> class L, typename T>
concept Indexable = requires (L<T> xs) { {xs[0]}; };

#endif
