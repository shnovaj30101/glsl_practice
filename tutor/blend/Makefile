all: blend blend2
clean:
	rm -f blend blend2 *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama -lassimp
LDFLAGS =

blend: blend.o 
	$(CXX) $(LDFLAGS) -o blend blend.o $(LIBS)
blend2: blend2.o 
	$(CXX) $(LDFLAGS) -o blend2 blend2.o $(LIBS)
