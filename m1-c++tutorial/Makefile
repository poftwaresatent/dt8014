#CXX = g++
CXX = clang++
CXXFLAGS = -Wall -O0 -g
PROGRAMS = $(shell ls *.cpp | sed 's:\.cpp$$::')

all:
	for prg in $(PROGRAMS); \
	do \
	  $(MAKE) recurse PRG=$$prg || exit 42; \
	done

recurse: $(PRG).cpp Makefile
	$(CXX) $(CXXFLAGS) -o $(PRG) $(PRG).cpp

clean:
	rm -fr *~ *.o *.dSYM $(PROGRAMS)
