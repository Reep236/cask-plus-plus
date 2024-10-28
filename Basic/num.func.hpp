#include <cmath>
#include <functional>

#ifndef NUM_FUNC_HPP
#define NUM_FUNC_HPP

template<typename T>
requires (std::is_arithmetic<T>::value) inline T abs_t(T x) { return std::abs(x); }

template<typename T>
requires (std::is_arithmetic<T>::value) inline T sig_t(T x) {return x == 0 ? 0:(x < 0 ? -1:1);}

template<typename T>
requires (std::is_arithmetic<T>::value) inline T negate_t(T x) { return std::negate<T>(x); }

template<typename T>
requires (std::is_arithmetic<T>::value) inline T fromInteger_t(int x) {return static_cast<T>(x);}

#endif
