all: depthtest 
clean:
	rm -f depthtest *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama -lassimp
LDFLAGS =

depthtest: depthtest.o 
	$(CXX) $(LDFLAGS) -o depthtest depthtest.o $(LIBS)
