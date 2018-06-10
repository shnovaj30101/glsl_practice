all: model_loading test
clean:
	rm -f model_loading test *.o
.PHONY: all clean

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lGLEW -lglad -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXcursor -lXinerama -lassimp
LDFLAGS =

model_loading: model_loading.o 
	$(CXX) $(LDFLAGS) -o model_loading model_loading.o $(LIBS)
test: test.o 
	$(CXX) $(LDFLAGS) -o test test.o $(LIBS)
