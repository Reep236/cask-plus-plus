CXX=g++
CPPFLAGS=-std=c++20 -Wall -Wextra -Werror -pedantic-errors 
CGFLAGS=-frelaxed-template-template-args
O=0
MAIN=main.cpp
HEADS=$(wildcard */*.hpp)
SRCS=$(wildcard */*.cpp)
TGT=dist/main
TGTCG=dist/mainClang
ARGS=1 1

main: Makefile $(HEADS) $(SRCS) $(MAIN)
	$(CXX) $(CPPFLAGS) -O$O -o $(TGT) $(SRCS) $(MAIN)

mainClang: Makefile $(HEADS) $(SRCS) $(MAIN)
	clang++ $(CPPFLAGS) $(CGFLAGS) -O$O -o $(TGTCG) $(SRCS) $(MAIN)

run: main
	./$(TGT) $(ARGS)

runClang: mainClang 
	./$(TGTCG) $(ARGS)

benchmark: $(HEADS) $(SRCS) benchmark.cpp
	$(CXX) $(CPPFLAGS) -O1 -o dist/bench $(SRCS) benchmark.cpp 
	dist/bench 16 2

clean:
	$(RM) $(TGT)
	$(RM) $(TGTCG)
	$(RM) dist/bench
