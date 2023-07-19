CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A08

A08: A08.cpp mazeGen.hpp
	g++ $(CFLAGS) -o A08 A08.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A08 >/dev/null 2>&1 || true
