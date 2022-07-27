all: integrateG
integrateG: integrateG.o
	g++ -pthread integrateG.o -o integrate
integrate.o: integrateG.cpp
	g++ -c integrateG.cpp