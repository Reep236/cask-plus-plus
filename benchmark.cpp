#include <string>
#include <chrono>

#include "prelude.hpp"

int main(int, char** argv) {
    int r1 = read<int>(argv[1]);
    int r2 = read<int>(argv[2]);
    auto l1 = std::views::iota(0, r1);
    auto l2 = std::views::iota(0, r2);
    auto ls = fconst(std::vector<int>(l2.begin(), l2.end()),std::vector<int>(l1.begin(), l1.end()));
    auto t00 = std::chrono::high_resolution_clock::now();
    auto sl = sequenceA(ls);
    auto t01 = std::chrono::high_resolution_clock::now();
    // A loop without any wrappers 
    for (auto xs : sl) {
        std::cout << "[";
        std::cout << xs.front();
        for (auto x : xs | std::views::drop(1)) {
            std::cout << ", ";
            std::cout << x;
        }
        std::cout << "]" << std::endl; 
    }
    auto t02 = std::chrono::high_resolution_clock::now();

    // A loop using print 
    for (auto x : sl)
        print(x)();
    auto t03 = std::chrono::high_resolution_clock::now();
    auto t04 = std::chrono::high_resolution_clock::now();

    // A loop lazily sequencing prints and then evaluating them
    IO<void*> actions0 = pure<IO, void*>(nullptr);
    for (auto x : sl | std::views::reverse) // Because in-place goes to b
     constbindIP(print(x), actions0);
    auto t14 = std::chrono::high_resolution_clock::now();
    actions0();
    auto t05 = std::chrono::high_resolution_clock::now();

    // Monadic map over the function
    mapM_(asF(print<std::vector<int>>), sl)();
    auto t06 = std::chrono::high_resolution_clock::now();

    std::cout << "sequenceA (us):";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t01-t00)).count() << std::endl;
    std::cout << "Standard Loop(us): ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t02-t01)).count() << std::endl;
    std::cout << "Print Loop(us): ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t03-t02)).count() << std::endl;
    std::cout << "In-place Sequencing Loop(us): ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t05-t04)).count() << std::endl;
    std::cout << "\tIn-place Sequencing Loop Construction(us): ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t14-t04)).count() << std::endl;
    std::cout << "\tIn-place Sequencing Loop Evaluation(us): ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t05-t14)).count() << std::endl;
    std::cout << "mapM_ (us): ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(t06-t05)).count() << std::endl;
    std::cout << std::endl;
    return 0;
}
