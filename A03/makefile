CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A03

A03: A03.cpp transforms.hpp
	g++ $(CFLAGS) -o A03 A03.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A03 >/dev/null 2>&1 || true
