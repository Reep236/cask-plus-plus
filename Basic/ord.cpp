#include "ord.hpp"
#include <stdexcept>

std::string show_t(Ordering x) {
    switch (x) {
        case Ordering::LT: return "LT";
        case Ordering::EQ: return "EQ";
        case Ordering::GT: return "GT";
    }
    throw std::invalid_argument("Non-Ord passed as Ordering"); // Complete, simply to satisfy -Wreturn-type
}

Ordering read_t(std::string x) {
    if (x == "LT") { return Ordering::LT; } 
    else if (x == "EQ") { return Ordering::EQ; } 
    else { return Ordering::GT; }
}

Ordering mappend_t(Ordering m, Ordering ms) {
    switch (m) {
        case Ordering::LT: return Ordering::LT;
        case Ordering::EQ: return ms;
        case Ordering::GT: return Ordering::GT;
    }
    throw std::invalid_argument("Non-Ord passed as Ordering");
}
