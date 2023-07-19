CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A06

A06: A06.cpp WorldView.hpp
	g++ $(CFLAGS) -o A06 A06.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A06 >/dev/null 2>&1 || true
