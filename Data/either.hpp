#include "either.func.hpp"

#ifndef EITHER_HPP
#define EITHER_HPP

template<typename Ta, typename Tb, typename Tc>
Tc either(std::function<Tc(Ta)> fl, std::function<Tc(Tb)> fr, Either<Ta, Tb> ex) {
    switch (ex.eitherT) {
        case (Either<Ta, Tb>::Left):  return fl(std::get<Ta>(ex.eitherVal));
        case (Either<Ta, Tb>::Right): return fr(std::get<Tb>(ex.eitherVal));
        default: throw std::invalid_argument("eitherT not in Left, Right");
    }
}

template<typename Ta, typename Tb>
std::list<Ta> lefts(std::list<Either<Ta, Tb>> xs) {
    std::list<Ta> ys;
    for (auto x : xs) if (x.eitherT == Either<Ta, Tb>::Left)
            ys.emplace_back(std::get<Ta>(x.eitherVal));
}

template<typename Ta, typename Tb>
std::list<Tb> rights(std::list<Either<Ta, Tb>> xs) {
    std::list<Tb> ys;
    for (auto x : xs) if (x.eitherT == Either<Ta, Tb>::Right)
            ys.emplace_back(std::get<Tb>(x.eitherVal));
}

template<typename Ta, typename Tb>
bool isLeft(Either<Ta, Tb> x) { return x.eitherT == Either<Ta, Tb>::Left; }

template<typename Ta, typename Tb>
bool isRight(Either<Ta, Tb> x) { return x.eitherT == Either<Ta, Tb>::Right; }

template<typename Ta, typename Tb>
Ta fromLeft(Ta a, Either<Ta, Tb> x) { return isLeft(x) ? std::get<Ta>(x.eitherVal):a; }

template<typename Ta, typename Tb>
Tb fromRight(Tb b, Either<Ta, Tb> x) { return isRight(x) ? std::get<Tb>(x.eitherVal):b; }

#endif
