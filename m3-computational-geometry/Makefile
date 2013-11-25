CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -O0 -g `pkg-config --cflags gtk+-2.0`
PROGRAMS = test-gfx test-polydraw test-intersect

all:
	for prg in $(PROGRAMS); \
	do \
	  $(MAKE) recurse PRG=$$prg; \
	done

recurse: $(PRG).o gfx.o Makefile
	$(CXX) $(CXXFLAGS) -o $(PRG) $(PRG).o gfx.o `pkg-config --libs gtk+-2.0`

gfx.o: gfx.cpp gfx.hpp Makefile
test-gfx.o: test-gfx.cpp gfx.hpp Makefile
test-polydraw.o: test-polydraw.cpp gfx.hpp Makefile
test-intersect.o: test-intersect.cpp gfx.hpp Makefile

clean:
	rm -rf *~ *.o *.dSYM $(PROGRAMS)
