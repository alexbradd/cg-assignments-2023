CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A11 shaders

A11: A11.cpp
	g++ $(CFLAGS) -o A11 A11.cpp $(LDFLAGS) -I "./headers"

shaders: shaders/BlinnFrag1.spv shaders/BlinnFrag2.spv shaders/BlinnFrag3.spv

shaders/BlinnFrag1.spv: shaders/BlinnShader1.frag
	glslc -o $@ shaders/BlinnShader1.frag

shaders/BlinnFrag2.spv: shaders/BlinnShader2.frag
	glslc -o $@ shaders/BlinnShader2.frag

shaders/BlinnFrag3.spv: shaders/BlinnShader3.frag
	glslc -o $@ shaders/BlinnShader3.frag

clean:
	rm A11 >/dev/null 2>&1 || true
	rm shaders/BlinnFrag*.spv >/dev/null 2>&1 || true
