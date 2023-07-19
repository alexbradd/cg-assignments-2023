CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A07

A07: A07.cpp Logic.hpp
	g++ $(CFLAGS) -o A07 A07.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A07 >/dev/null 2>&1 || true
