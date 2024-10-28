#include "maybe.func.hpp"
#include "monoid.hpp"

#ifndef MAYBE_HPP
#define MAYBE_HPP

// Helps supress uninitialized errors (e.g. String)
template<typename T>
requires (Monoid<T>) inline const Maybe<T> nothing<T> = {Maybe<T>::Nothing, mempty<T>};

// The `maybe` function takes a defualt value, a function, and a `Maybe` value. 
// If the `Maybe` value is `Nothing`, the function returns the default value. 
// Otherwise, it applies the function to the value inside the `Just` and returns the result. 
template<typename Ta, typename Tb>
Tb maybe(Tb dft, std::function<Tb(Ta)> f, Maybe<Ta> x) {
    switch(x.maybeVal) {
        case Maybe<Ta>::Just: return f(x.justVal);
        case Maybe<Ta>::Nothing: return dft;
        default: throw std::invalid_argument("maybeVal not in Nothing, Just");
    }
}

// The `isJust` function returns True iff its argument is of the form `Just` _.
template<typename T>
bool isJust(Maybe<T> x) { return x.maybeVal == Maybe<T>::Just; } 

// The `isNothing` function returns `True` iff its argument is `Nothing`.
template<typename T> 
bool isNothing(Maybe<T> x) { return x.maybeVal == Maybe<T>::Nothing; }

// The `fromJust` function extracts the element out of a `Just` and throws an error if its argument is `Nothing`
template<typename T> 
T fromJust(Maybe<T> x) { 
    switch (x.maybeVal) {
        case Maybe<T>::Just: return x.justVal;
        case Maybe<T>::Nothing: throw std::invalid_argument("Expected \"Just\" in fromJust, got \"Nothing\"");
        default: throw std::invalid_argument("maybeVal not in Nothing, Just");
    }
}

// The `fromMaybe` function takes a default value and a `Maybe` value. 
// If the `Maybe` is `Nothing`, it returns the default value; otherwise, it returns a value contained in the `Maybe`.
template<typename T>
T fromMaybe(T dft, Maybe<T> x) { 
    switch (x.maybeVal) { 
        case Maybe<T>::Just: return x.justVal;
        case Maybe<T>::Nothing: return dft; 
        default: throw std::invalid_argument("maybeVal not in Nothing, Just"); 
    }
}

// The `listToMaybe` function returns `Nothing` on an empty list or `Just` `a` where `a` is the first element of the list.
template<typename T>
Maybe<T> listToMaybe(std::list<T> x) {
    return x.empty() ? nothing<T>:just(x.front());
}

// The `maybeToList` function returns an empty list when given `Nothing` or a singleton list when given `Just`.
template<typename T>
std::list<T> maybeToList(Maybe<T> mx) {
    return maybe(std::list<T> {}, [] (T a) -> std::list<T> { return {a}; }, mx);
}

// The `catMaybes` function takes a list of `Maybe`s and returns a list of all the `Just` values.
template<typename T>
std::list<T> catMaybes(std::list<Maybe<T>> mxs) {
    std::list<T> xs;
    for (Maybe<T> mx : mxs) {
        switch (mx.maybeVal) {
            case Maybe<T>::Just: {
                xs.emplace_back(mx.justVal);
                break;
            }
            case Maybe<T>::Nothing: break;
            default: break;
        }
    }
    return xs;
}

// The `mapMaybe` function is a version of map which can throw out elements. In particular, the functional argument returns something of type `Maybe<Tb>`.
// If this is `Nothing`, no element is added on to the result list. If it is `Just` `b`, then `b` is included in the result list. 
template<typename Ta, typename Tb>
std::list<Tb> mapMaybe(std::function<Maybe<Tb>(Ta)> f, std::list<Ta> xs) {
    std::list<Tb> ys; 
    for (Ta x : xs) {
        Maybe<Tb> my = f(x);
        switch (my.maybeVal) {
            case Maybe<Tb>::Just: {
                ys.emplace_back(my.justVal);
                break;
            }
            case Maybe<Tb>::Nothing: break;
            default: break;
        }
    }
    return ys;
}

template<typename T>
Maybe<std::pair<T, std::list<T>>> uncons(std::list<T> xs) {
    if (xs.empty()) return nothing<std::pair<T, std::list<T>>>;
    T h = xs.front();
    xs.pop_front();
    return just(std::pair<T, std::list<T>>{h, xs});
} 

template<typename T>
Maybe<std::pair<std::list<T>, T>> unsnoc(std::list<T> xs) {
    if (xs.empty()) return nothing<std::pair<std::list<T>, T>>;
    T t = xs.back();
    xs.pop_back();
    return just(std::pair<std::list<T>, T>{xs, t});
}


#endif
