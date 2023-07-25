CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A14

A14: A14.cpp primGen.hpp
	g++ $(CFLAGS) -o A14 A14.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A14 >/dev/null 2>&1 || true
