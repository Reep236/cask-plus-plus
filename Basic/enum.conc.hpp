#include <concepts>
#include "enum.func.hpp"

#ifndef ENUM_CONC_HPP
#define ENUM_CONC_HPP

template<typename T>
concept Enum = requires (T x) {
    { toEnum_t(&x, 0) };
    { fromEnum_t(x) };
};

template<typename T>
concept Bounded = requires (T x) {
    { minBound_t<T> };
    { maxBound_t<T> };
};

#endif
