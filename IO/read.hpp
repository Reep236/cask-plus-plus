#include <exception>

#include "read.func.hpp"
#include "read.conc.hpp"
#include "../Data/maybe.hpp"
#include "../Data/either.hpp"

#ifndef READ_HPP
#define READ_HPP

template<typename T>
requires (Read<T>) T _read(T* phantom, const std::string& s) {
    return read_t(phantom, s); 
};

template<template<typename ...> class C, typename T>
requires (Read1<C, T>) C<T> _read(C<T>* phantom, const std::string& s) {
    return read_c(asF(_read<T>), phantom, s);
}

template<template<typename ...> class C1, template<typename ...> class C2, typename T>
requires (Read1<C1, T>, Read1<C2, T>) C1<C2<T>> _read(C1<C2<T>>* phantom, const std::string& s) {
    return read_c(asF(_read<C2, T>), phantom, s);
}

template<template<typename, typename> class C, typename T1, typename T2>
requires (Read2<C, T1, T2>) C<T1, T2> _read(C<T1, T2>* phantom, const std::string& s) {
    return read_c2(asF(_read<T1>),
                   asF(_read<T2>), phantom, s);
}

template<template<typename, typename> class CS, template<typename ...> class C1,  typename T1, typename T2>
requires (Read1<C1, T1> && Read2<CS, T1, T2>) CS<C1<T1>, T2> _read(CS<C1<T1>, T2>* phantom, const std::string& s) {
    return read_c2(asF(_read<C1, T1>), asF(_read<T2>), phantom, s);
}

template<template<typename, typename> class CS, template<typename ...> class C2,  typename T1, typename T2>
requires (Read1<C2, T2> && Read2<CS, T1, T2>) CS<T1, C2<T2>> _read(CS<T1, C2<T2>>* phantom, const std::string& s) {
    return read_c2(asF(_read<T1>), asF(_read<C2, T2>), phantom, s);
}

// Read value from a string
template<typename T>
T read(const std::string& s) { return _read((T*)nullptr, s); }

template<typename T>
Maybe<T> readMaybe(const std::string& s) { 
    try {
        return just(read<T>(s));
    } catch (std::exception) {
        return nothing<T>;
    }
}

template<typename T>
Either<std::string, T> readEither(const std::string& s) {
    try { 
        return right<std::string>(read<T>(s));
    } catch (std::exception) {
        return left<std::string, T>("read: no parse");
    }
}



#endif
