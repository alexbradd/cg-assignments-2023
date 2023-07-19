CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A00

A00: A00.cpp
	g++ $(CFLAGS) -o A00 A00.cpp $(LDFLAGS) -I "./headers"

clean:
	rm A00 >/dev/null 2>&1 || true
