all: main main2 main3
clean:
	rm -f main main2 main3 *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama
LDFLAGS =

main: main.o
	$(CXX) $(LDFLAGS) -o main main.o $(LIBS)
main2: main2.o
	$(CXX) $(LDFLAGS) -o main2 main2.o $(LIBS)
main3: main3.o
	$(CXX) $(LDFLAGS) -o main3 main3.o $(LIBS)
