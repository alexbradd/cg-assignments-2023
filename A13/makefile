CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A13 shaders

A13: A13.cpp
	g++ $(CFLAGS) -o A13 A13.cpp $(LDFLAGS) -I "./headers"

shaders: shaders/BlinnFrag.spv

shaders/BlinnFrag.spv: shaders/BlinnShader.frag
	glslc -o $@ shaders/BlinnShader.frag

clean:
	rm A13 >/dev/null 2>&1 || true
	rm shaders/BlinnFrag.spv >/dev/null 2>&1 || true
