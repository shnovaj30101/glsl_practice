all: lightcast lightcast2 lightcast3
clean:
	rm -f lightcast lightcast2 lightcast3 *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama
LDFLAGS =

lightcast: lightcast.o 
	$(CXX) $(LDFLAGS) -o lightcast lightcast.o $(LIBS)
lightcast2: lightcast2.o 
	$(CXX) $(LDFLAGS) -o lightcast2 lightcast2.o $(LIBS)
lightcast3: lightcast3.o 
	$(CXX) $(LDFLAGS) -o lightcast3 lightcast3.o $(LIBS)
