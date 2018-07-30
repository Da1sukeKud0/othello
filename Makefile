othe_forMac: othe_forMac.cpp
	g++ -std=c++11 -Wall othe_forMac.cpp -o run_forMac
othe: othe.cpp
	g++ -std=c++11 -Wall othe.cpp -o run
clean:
	rm -f *.o
