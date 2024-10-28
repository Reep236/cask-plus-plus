Haskell Type Classes in C++ Concepts


# Definitions 

1. **X.func.hpp** contains declarations and basic instances for the simplest minimal complete definition of a Haskell type class, X 
2. **X.conc.hpp** contains the definition of Haskell type class, X, as a C++ Concept. In a well-formed class, X.conc.hpp will include X.func.hpp
3. **X.hpp** includes both X.func.hpp (explicity by convention, though unneccessary) and X.conc.hpp, along with any class functions not in the minimal complete definition 


# Rules
1. Definitions in \_.func.hpp may **NOT** reference **any** concepts
2. All Haskell datatypes / C++ classes must be defined in \_.func.hpp 
3. Appropriate instance definitions for **stdlib types** are to be included in \_.func.hpp
4. Appropriate class instances for datatypes defined in a \_.func.hpp file are to be included with the datatype definition, a \_.func.hpp include is permitted only to provide necessary datatype definitions.  

