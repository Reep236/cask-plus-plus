#include "num.func.hpp"
#include "num.conc.hpp"

#ifndef NUM_HPP
#define NUM_HPP

template<typename T>
requires (Num<T>) inline T absN(T x) {return abs_t(x);}

template<typename T>
requires (Num<T>) inline T sigN(T x) {return sig_t(x);}

template<typename T>
requires (Num<T>) inline T negateN(T x) {return negate_t(x);}

template<typename T>
requires (Num<T>) inline T fromInteger(int x) {return fromInteger<T>(x);}

#endif
