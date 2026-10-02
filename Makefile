CXX=g++

CFLAGS = -Wall -Wextra -g -03
LIBS = -lraylib
BIN = flocking_sim.exe

all:
	g++ -shared -fPIC -g boids.cpp -o boids.so.tmp
	mv boids.so.tmp boids.so

platform:
	$(CXX) $(CFLAGS) main.cpp -o $(BIN) $(LIBS)
	

