#include "num.func.hpp"

#ifndef NUM_CONC_HPP
#define NUM_CONC_HPP

template<typename T>
concept Num = requires (T x) {
    {x+x};
    {x*x};
    {abs_t(x)};
    {sig_t(x)};
    {negate_t(x)};
    {fromInteger_t<T>(0)};
};

#endif
