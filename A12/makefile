CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

all: A12 shaders

A12: A12.cpp
	g++ $(CFLAGS) -o A12 A12.cpp $(LDFLAGS) -I "./headers"

shaders: shaders/PhongFrag.spv shaders/OrenNayarFrag.spv shaders/BlinnNormMapFrag.spv shaders/GGXNormMapFrag.spv

shaders/PhongFrag.spv: shaders/PhongShader.frag
	glslc -o $@ shaders/PhongShader.frag

shaders/OrenNayarFrag.spv: shaders/OrenNayarShader.frag
	glslc -o $@ shaders/OrenNayarShader.frag

shaders/BlinnNormMapFrag.spv: shaders/BlinnNormMapShader.frag
	glslc -o $@ shaders/BlinnNormMapShader.frag

shaders/GGXNormMapFrag.spv: shaders/GGXNormMapShader.frag
	glslc -o $@ shaders/GGXNormMapShader.frag

clean:
	rm A12 >/dev/null 2>&1 || true
	rm shaders/PhongFrag.spv >/dev/null 2>&1 || true
	rm shaders/OrenNayarFrag.spv >/dev/null 2>&1 || true
	rm shaders/BlinnNormMapFrag.spv >/dev/null 2>&1 || true
	rm shaders/GGXNormMapFrag.spv >/dev/null 2>&1 || true
