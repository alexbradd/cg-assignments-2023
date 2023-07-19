CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A05

A05: A05.cpp projection.hpp
	g++ $(CFLAGS) -o A05 A05.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A05 >/dev/null 2>&1 || true
