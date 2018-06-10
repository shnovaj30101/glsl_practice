all: shader1 shader2
clean:
	rm -f shader1 shader2 *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama
LDFLAGS =

shader1: shader1.o
	$(CXX) $(LDFLAGS) -o shader1 shader1.o $(LIBS)
shader2: shader2.o
	$(CXX) $(LDFLAGS) -o shader2 shader2.o $(LIBS)
