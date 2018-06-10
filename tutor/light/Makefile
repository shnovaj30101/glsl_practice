all: light 
clean:
	rm -f light  *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama
LDFLAGS =

light: light.o 
	$(CXX) $(LDFLAGS) -o light light.o $(LIBS)
