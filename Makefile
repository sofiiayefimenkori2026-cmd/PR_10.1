CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: Lab_10_1 UnitTest_10_1

Lab_10_1: Lab_10_1.cpp
	$(CXX) $(CXXFLAGS) Lab_10_1.cpp -o $@

UnitTest_10_1: UnitTest_10_1.cpp Lab_10_1.cpp doctest.h
	$(CXX) $(CXXFLAGS) UnitTest_10_1.cpp -o $@

check: UnitTest_10_1
	./UnitTest_10_1

clean:
	rm -rf Lab_10_1 UnitTest_10_1 *.dSYM
