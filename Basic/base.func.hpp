#include <concepts>
#include <functional>

#ifndef BASE_FUNC_HPP
#define BASE_FUNC_HPP

// Writing std::function... gets to be a pain and nasty, inline means minimal overhead  

// Option B for deduction, standard function pointer
template<typename Res, typename ...Args>
inline std::function<Res(Args...)> asF(Res(*f)(Args...)) { return std::function<Res(Args...)>(f); }

// Option C for deduction, should catch everything else
template<typename Res, typename ...Args, typename C>
inline std::function<Res(Args...)> asF(Res(C::*f)(Args...)) { return std::function<Res(Args...)>(f); }

// Option D for deduction, if a given Callable (e.g. bind_front) is being stubborn
template<typename Res, typename ...Args, typename T>
requires(std::invocable<T, Args...>) inline std::function<Res(Args...)> asF(T&& f) { return std::function<Res(Args...)>(f); }

// Identity function
template<typename T>
inline const std::function<T(T)> id = std::function<T(T)>(std::identity());

// Special case of bind_front for 1 argument, allows defined std::function return 
template<typename Ta, typename Tb, typename ... Args> 
inline std::function<Tb(Args...)> partial(std::function<Tb(Ta, Args ...)> f, Ta x) { 
    return asF<Tb, Args...>(std::bind_front(f, x));
}

// Otherwise std::function would have to be called twice 
template<typename Ta, typename Tb, typename ... Args> 
inline std::function<Tb(Args ...)> partial(Tb(*f)(Ta, Args ...), Ta x) { 
    return asF<Tb, Args...>(std::bind_front(f, x));
}

// const(x, y) always evaluates to x, ignoring its second argument
// template args flipped so one must only specify constf<Tb>
template<typename Tb, typename Ta> 
inline Ta constf(Ta x, Tb) { return x; }

// Since const is so often partially applied 
template<typename Tb, typename Ta>
inline std::function<Ta(Tb)> constf(Ta x) { return partial(static_cast<Ta(*)(Ta,Tb)>(constf), x); }

// Right to left function composition
// Simplest case of (.), with no additional arguments to either function  
template<typename Ta, typename Tb, typename Tc>
inline std::function<Tc(Ta)> compose(std::function<Tc(Tb)> f, std::function<Tb(Ta)> g) {
    return asF([&f,&g] (Ta x) {return f(g(x));});
}

template<typename Ta, typename Tb, typename Tc, typename ... Args>
inline std::function<Tc(Ta, Args...)> compose(std::function<Tc(Tb, Args...)> f, std::function<Tb(Ta)> g) {
    return asF([&f,&g] (Ta x, Args... args) {return f(g(x), std::forward<Args>(args)...);});
}

template<typename Ta, typename Tb>
inline std::function<Tb()> compose(std::function<Tb(Ta)> f, std::function<Ta()> g) {
    return asF<Tb>([&f,&g] {return f(g());});
}

// flip(f) takes its (first) two arguments in the reverse order of f 
template<typename T1, typename T2, typename T3>
inline std::function<T3(T2, T1)> flip(std::function<T3(T1, T2)> f) {
    return asF<T3, T2, T1>([&f] (T2 b, T1 a) -> T3 { return f(a, b); });
}

template<typename T1, typename T2, typename T3, typename ... Args>
inline std::function<T3(T2, T1, Args...)> flip(std::function<T3(T1, T2, Args...)> f) {
    return asF([&f] (T2 b, T1 a, Args... args) {return f(a, b, std::forward<Args>(args)...);});
} 

template<typename Ta>
inline Ta until(std::function<bool(Ta)> p, std::function<Ta(Ta)> f, Ta x) {
    return p(x) ? x:until(p, f, f(x)); 
}

#endif
