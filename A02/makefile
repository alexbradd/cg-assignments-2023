CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A02

A02: A02.cpp transforms.hpp
	g++ $(CFLAGS) -o A02 A02.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A02 >/dev/null 2>&1 || true
