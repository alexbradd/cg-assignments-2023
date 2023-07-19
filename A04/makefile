CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A04

A04: A04.cpp projection.hpp
	g++ $(CFLAGS) -o A04 A04.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A04 >/dev/null 2>&1 || true
