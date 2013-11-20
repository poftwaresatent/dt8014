CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -O0 -g `pkg-config --cflags gtk+-2.0`

all: test-gfx test-polydraw

test-gfx: test-gfx.o gfx.o Makefile
	$(CXX) $(CXXFLAGS) -o test-gfx test-gfx.o gfx.o `pkg-config --libs gtk+-2.0`

test-polydraw: test-polydraw.o gfx.o Makefile
	$(CXX) $(CXXFLAGS) -o test-polydraw test-polydraw.o gfx.o `pkg-config --libs gtk+-2.0`

gfx.o: gfx.cpp gfx.hpp Makefile
test-gfx.o: test-gfx.cpp gfx.hpp Makefile
test-polydraw.o: test-polydraw.cpp gfx.hpp Makefile

clean:
	rm -rf *~ *.o *.dSYM test-gfx test-polydraw
