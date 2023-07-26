CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A15

A15: A15.cpp primGen.hpp
	g++ $(CFLAGS) -o A15 A15.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A15 >/dev/null 2>&1 || true
