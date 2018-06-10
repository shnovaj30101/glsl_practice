all: camera camera2
clean:
	rm -f camera camera2 *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama
LDFLAGS =

camera: camera.o
	$(CXX) $(LDFLAGS) -o camera camera.o $(LIBS)
camera2: camera2.o
	$(CXX) $(LDFLAGS) -o camera2 camera2.o $(LIBS)
