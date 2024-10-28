#include <concepts>
#include "monoid.func.hpp"
#include "semigroup.hpp"

#ifndef MONOID_CONC_HPP
#define MONOID_CONC_HPP

// The class of monoids (types with an associative binary operation that has an identity). 
template<typename M>
concept Monoid = requires (M x) {
    {sappend(x, _mempty<M>)};
};

#endif
