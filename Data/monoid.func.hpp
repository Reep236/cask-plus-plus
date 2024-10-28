#include <forward_list>
#include <list>
#include <string>
#include <vector>
#include <deque>
#include <unordered_set>
#include <set>

#include "semigroup.func.hpp"

#ifndef MONOID_FUNC_HPP
#define MONOID_FUNC_HPP

// Identity of sappend. 
template<typename M>
inline const M _mempty;

template<typename T>
inline const std::vector<T> _mempty<std::vector<T>> = {};

template<typename T>
inline const std::deque<T> _mempty<std::deque<T>> = {};

template<typename T>
inline const std::forward_list<T> _mempty<std::forward_list<T>> = {};

template<typename T>
inline const std::list<T> _mempty<std::list<T>> = {};

template<typename T>
inline const std::unordered_set<T> _mempty<std::unordered_set<T>> = {};

template<typename T>
inline const std::set<T> _mempty<std::set<T>> = {};

template<>
inline const std::string _mempty<std::string> = "";

#endif
