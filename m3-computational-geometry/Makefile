CXX = g++
CXXFLAGS = -Wall -O0 -g -pipe `pkg-config --cflags gtk+-2.0`

all: test-gfx

test-gfx: test-gfx.o gfx.o Makefile
	$(CXX) $(CXXFLAGS) -o test-gfx test-gfx.o gfx.o `pkg-config --libs gtk+-2.0`

gfx.o: gfx.cpp gfx.hpp
test-gfx.o: gfx.hpp

clean:
	rm -rf *~ *.o *.dSYM test-gfx
