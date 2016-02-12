all:
	c++ -O3 -Wall -std=c++14 -lcurses -o neutrino $(wildcard *.cpp)

clean:
	rm -f neutrino
