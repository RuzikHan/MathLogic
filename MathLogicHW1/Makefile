CXXFLAGS+=-std=c++11
CXX=g++

all: out out/main.o

run: out out/main.o
		./out/main.o

out/main.o: src/main.cpp
		$(CXX) $^ $(CXXFLAGS) -o out/main.o

out:
		mkdir -p out

clean:
		rm -rf out