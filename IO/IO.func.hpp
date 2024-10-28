#include <stdexcept>

#include "../Basic/base.func.hpp"
#include "../Data/maybe.func.hpp"
#include "../Data/functor.func.hpp"
#include "../Control/applicative.func.hpp"
#include "../Control/monad.func.hpp"

#ifndef IO_FUNC_HPP
#define IO_FUNC_HPP

template<typename T>
struct IO {
    std::vector<std::function<T()>> runIO;
    Maybe<T> res = nothing<T>;
    
    IO() : runIO() {}

    IO(std::function<T()> f) : runIO{f} {}
    
    template<class C>
    IO(T(C::*f)()) : runIO{asF<T>(f)} {}

    IO(T res) {
        this->res   = just(res);
    }
    
    template<typename Tdisc>
    IO(IO<Tdisc> discard, const IO<T>& keep) {
        this->runIO = keep.runIO;
        this->res   = keep.res;
        fuse(discard);
    } 

    IO(const IO<T>& discard, const IO<T>& keep) {
        this->runIO = keep.runIO;
        this->res   = keep.res;
        fuse(discard);
    }

    T operator() () {
        switch (this->res.maybeVal) {
            case Maybe<T>::Just: break;
            case Maybe<T>::Nothing: {
                T xres;
                do {
                    xres = runIO.back()();
                    runIO.pop_back();
                } while (!runIO.empty());

                this->res = just(xres);
            }
        }
        return this->res.justVal;
    }

    template<typename Tdisc>
    void fuse(IO<Tdisc> x) {
        switch (x.res.maybeVal) {
            case Maybe<T>::Just: break;
            case Maybe<T>::Nothing: {
                    this->runIO.push_back(asF<T>([x,this] () mutable -> T {x(); return this->res.justVal;}));
                    this->res   = nothing<T>;
                    break;
            }      

        };
    }
    
    void fuse(const IO<T>& x) {
        if (this->runIO.empty())
            runIO.push_back(asF<T>([this] () -> T {return this->res.justVal;}));
        this->runIO.insert(this->runIO.end(), x.runIO.begin(), x.runIO.end());
        this->res   = nothing<T>;
    }
};

template<typename Ta, typename Tb>
inline IO<Tb> fmap_c(std::function<Tb(Ta)> f, IO<Ta> x) {
    return IO(std::function<Tb()>([x,f] () mutable { return f(x()); }));
}

template<typename T>
inline IO<T> pure_c(IO<T>*, T x) {
    return IO(x);
}

template<typename Ta, typename Tb>
inline IO<Tb> ap_c(IO<std::function<Tb(Ta)>> f, IO<Ta> x) {
    return IO(std::function<Tb()>([x,f] () mutable { return f()(x()); }));
}

template<typename Ta, typename Tb>
inline IO<Tb> lazyAp_c(IO<std::function<Tb(Ta)>> f, std::function<IO<Ta>()> x) {
    return IO(std::function<Tb()>([x,f] () mutable { return f()(x()()); }));
}

template<typename T>
inline const IO<T> _emptyA<IO<T>> = throw std::runtime_error("mzero/emptyA IO");

template<typename T>
IO<T> aplus_c(IO<T> a, IO<T> as) { 
    return IO(std::function<T()>([a, as] () mutable {
                try {return a();}
                catch (std::exception const&) {return as();}
             }));
}

// Have to reconcile two branches of T()...
template<typename Ta, typename Tb, typename Targ>
requires (std::same_as<Targ, Ta> || std::same_as<Targ, const Ta&>)
    inline IO<Tb> bind_c(IO<Ta> x, std::function<IO<Tb>(Targ)> f) {
        return IO(std::function<Tb()>([x,f] () mutable { return f(x())(); })); // Surely there's a better way of doing this
    }

// "a" shouldn't be interpreted as a const&, as it is passed by value at a later point
// However, this significantly improves performance
template<typename Ta, typename Tb>
inline IO<Tb> constbind(const IO<Ta>& a, const IO<Tb>& b) {
    return IO(a, b);
}

template<typename Ta, typename Tb>
inline void constbindIP(IO<Ta> a, IO<Tb>& b) {
    b.fuse(a);
} 

#endif
