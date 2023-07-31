CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A16

A16: A16.cpp Starter.hpp
	g++ $(CFLAGS) -o A16 A16.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A16 >/dev/null 2>&1 || true
