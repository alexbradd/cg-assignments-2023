CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A01

A01: A01.cpp triangles.hpp
	g++ $(CFLAGS) -o A01 A01.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A01 >/dev/null 2>&1 || true
