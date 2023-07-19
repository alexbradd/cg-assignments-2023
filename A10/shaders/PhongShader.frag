// vim: ft=glsl
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform GlobalUniformBufferObject {
	float time;
} gubo;

void main() {
	vec3 Norm = fragNorm;

	float y = fragPos.y;
	float t = gubo.time;
	float r = 0.0f, g = 0.0f, b = 0.0f;

	/***** Insert the code here to compute the correct value of r, g and b ****/
  float h = mod((t / 3) + y, 6);
  if (h < 1.0) {
    r = 1;
    g = h;
    b = 0;
  } else if (1.0 <= h && h < 2.0) {
    r = 2 - h;
    g = 1;
    b = 0;
  } else if (2.0 <= h && h < 3.0) {
    r = 0;
    g = 1;
    b = h - 2;
  } else if (3.0 <= h && h < 4.0) {
    r = 0;
    g = 4 - h;
    b = 1;
  } else if (4.0 <= h && h < 5.0) {
    r = h - 4;
    g = 0;
    b = 1;
  } else {
    r = 1;
    g = 0;
    b = 6 - h;
  }
	/***** Leave it as is from this point on ****/

	outColor = vec4(r, g, b, 1.0);
}
