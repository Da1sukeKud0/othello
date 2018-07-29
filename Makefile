othe: othe.cpp
	g++ -std=c++11 -Wall othe.cpp -o run
othe_forMac: othe_forMac.cpp
	g++ -std=c++11 -Wall othe_forMac.cpp -o run_forMac
clean:
	rm -f *.o
