all: stenciltest
clean:
	rm -f stenciltest *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama -lassimp
LDFLAGS =

stenciltest: stenciltest.o 
	$(CXX) $(LDFLAGS) -o stenciltest stenciltest.o $(LIBS)
