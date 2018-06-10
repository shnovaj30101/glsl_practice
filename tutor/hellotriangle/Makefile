all: hellotriangle pratice1 pratice2
clean:
	rm -f hellotriangle pratice1 pratice2 *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama
LDFLAGS =

hellotriangle: hellotriangle.o
	$(CXX) $(LDFLAGS) -o hellotriangle hellotriangle.o $(LIBS)
pratice1: pratice1.o
	$(CXX) $(LDFLAGS) -o pratice1 pratice1.o $(LIBS)
pratice2: pratice2.o
	$(CXX) $(LDFLAGS) -o pratice2 pratice2.o $(LIBS)
