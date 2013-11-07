#CXX = g++
CXX = clang++
CXXFLAGS = -Wall -O0 -g

all: ringbuffer-ex1 ringbuffer-ex2 template-error-ex1 template-error-ex2

ringbuffer-ex1: ringbuffer-ex1.o Makefile
	$(CXX) $(CXXFLAGS) -o ringbuffer-ex1 ringbuffer-ex1.o

ringbuffer-ex2: ringbuffer-ex2.o Makefile
	$(CXX) $(CXXFLAGS) -o ringbuffer-ex2 ringbuffer-ex2.o

template-error-ex1: template-error-ex1.o Makefile
	$(CXX) $(CXXFLAGS) -o template-error-ex1 template-error-ex1.o

template-error-ex2: template-error-ex2.o Makefile
	$(CXX) $(CXXFLAGS) -o template-error-ex2 template-error-ex2.o

clean:
	rm -rf *~ *.o *.dSYM ringbuffer-ex1 ringbuffer-ex2 template-error-ex1 template-error-ex2
