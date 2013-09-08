CXX = g++
CXXFLAGS = -Wall -O0 -g -pipe `pkg-config --cflags gtk+-2.0`

all: classes-start test-gfx

test-gfx: test-gfx.o gfx.o Makefile
	$(CXX) $(CXXFLAGS) -o test-gfx test-gfx.o gfx.o `pkg-config --libs gtk+-2.0`

classes-start: classes-start.o Makefile
	$(CXX) $(CXXFLAGS) -o classes-start classes-start.o

gfx.o: gfx.cpp gfx.hpp

clean:
	rm -rf *~ *.o *.dSYM classes-start test-gfx
