CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -O0 -g

PROGRAMS = hashbench rbtreebench p1-simplistic

all: programs

programs:
	for prg in $(PROGRAMS); \
	do \
	  $(MAKE) recurse PRG=$$prg; \
	done

recurse: $(PRG).cpp Makefile
	$(CXX) $(CXXFLAGS) -o $(PRG) $(PRG).cpp

clean:
	rm -fr *~ *.o *.dSYM $(PROGRAMS)
