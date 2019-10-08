CXXFLAGS=-std=c++11 -I../
CFLAGS=-std=c99 -I../
CC=gcc

all: standalone wrapper 

standalone:
	$(CXX) $(CXXFLAGS) -c ubifs_fuzzer.cc
	$(CXX) $(CXXFLAGS) -c ubifs_standalone.cc
	$(CXX) $(CXXFLAGS) -o ubifs_standalone ubifs_standalone.o ubifs_fuzzer.o

wrapper:
	$(CXX) -fPIC $(CXXFLAGS) -c ubifs_fuzzer.cc
	$(CXX) -fPIC $(CXXFLAGS) -c ubifs_wrapper.cc
	$(CXX) -fPIC -shared $(CXXFLAGS) -o ubifs_wrapper.so ubifs_wrapper.o ubifs_fuzzer.o

clean:
	rm -f *.o
	rm -f *_standalone
	rm -f ubifs_wrapper.so
	rm -f meta.img out.img 
