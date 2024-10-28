#include "monoid.func.hpp"
#include "monoid.conc.hpp"
#include <list>

#ifndef MONOID_HPP
#define MONOID_HPP

template<typename M> 
requires (Monoid<M>) inline const M mempty = _mempty<M>;

// Just sappend with an extra Monoid requirement 
template<typename M>
requires (Monoid<M>) inline M mappend(M m, M ms) { return sappend(m, ms); }

// Fold a list using the monoid 
template<typename M>
M mconcat(std::list<M> mls) {
    M ms = mempty<M>;
    for (auto m : mls) {
        ms = mappend(m, ms);
    };
    return ms;
}

#endif
