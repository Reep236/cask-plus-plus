#include <limits>
#include <type_traits>

#ifndef ENUM_FUNC_HPP
#define ENUM_FUNC_HPP

template<typename T>
constexpr T minBound_t;

template<typename T>
constexpr T maxBound_t;

template<typename T>
requires (std::is_arithmetic<T>::value) constexpr T minBound_t<T> = std::numeric_limits<T>::min();

template<typename T>
requires (std::is_arithmetic<T>::value) constexpr T maxBound_t<T> = std::numeric_limits<T>::max();

template<typename T>
inline T toEnum_t(T*, int i) { return static_cast<T>(i); }

template<typename T>
inline int fromEnum_t(T a) { return static_cast<int>(a); }

#endif
