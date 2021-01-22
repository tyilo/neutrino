all:
	c++ -O3 -Wall -std=c++17 -lcurses -o neutrino ai.cpp display.cpp neutrino.cpp main.cpp
	c++ -O3 -Wall -std=c++17 -o test neutrino.cpp test.cpp

clean:
	rm -f neutrino
	rm -rf test
