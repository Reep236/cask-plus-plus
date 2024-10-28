#include "prelude.hpp"

int main() {
    // We can run standalone IO actions 
    putStrLn("Welcome to Cask++!")();

    // Store and chain their values to run later
    // Note that Clang is occasionally unkind with type inference... g++ will compile without the static_cast
    auto ioxs = putStrLn("Input a value:") 
             >> fmap( partial(static_cast<std::vector<int>(*)(int, int)>(enumFromTo), 1)
                    , readln<int>());
    
    // But IO actions will only be evaluated once!
    ioxs();
    putStrLn("Computing...");
    auto xs = ioxs();
    auto combos = sequenceA(fconst(xs, xs)); // Sometimes we miss composition with (.)

    // std::function is used when we want to pass around functions
    // It can get a bit bulky, so asF is shorthand that will try to deduce the type
    auto printVec = asF(print<const std::vector<int>&>);

    mapM_(printVec, combos)();
    
    return 0;
}
