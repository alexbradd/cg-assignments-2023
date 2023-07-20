#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightPos;   // position of the spotlight
	vec3 lightDir;   // direction of the spotlight
	vec4 lightColor; // color of the spotlight
	vec3 eyePos;     // position of the viewer
} gubo;

layout(binding = 3) uniform sampler2D texEmit;

const float beta = 2.0f;   // decay exponent of the spotlight
const float g = 3;         // target distance of the spotlight
const float cosout = 0.5;  // cosine of the outer angle of the spotlight
const float cosin  = 0.95; // cosine of the inner angle of the spotlight

const float gamma = 160.0f;	// cosine power for the Blinn specular reflection

// coefficients for the spehrical harmonics ambient light term
const vec3 C00  = vec3( .38f, .43f, .45f)/8.0f;
const vec3 C1m1 = vec3( .29f, .36f, .41f)/8.0f;
const vec3 C10  = vec3( .04f, .03f, .01f)/8.0f;
const vec3 C11  = vec3(-.10f,-.10f,-.09f)/8.0f;
const vec3 C2m2 = vec3(-.06f,-.06f,-.04f)/8.0f;
const vec3 C2m1 = vec3( .01f,-.01f,-.05f)/8.0f;
const vec3 C20  = vec3(-.09f,-.13f,-.15f)/8.0f;
const vec3 C21  = vec3(-.06f,-.05f,-.04f)/8.0f;
const vec3 C22  = vec3( .02f, .00f,-.05f)/8.0f;

vec3 spotlight(vec3 unNormLx, vec3 lx) {
  float num = dot(lx, gubo.lightDir) - cosout;
  float denom = cosin - cosout;
  float coneDimming = clamp(num / denom, 0.0, 1.0);
  vec3 lightDimming = pow(g / length(unNormLx), beta) * gubo.lightColor.rgb;
	return lightDimming * coneDimming;
}

vec3 blinn(vec3 L, vec3 N, vec3 V, vec3 MD, vec3 MS) {
  vec3 diffuse = max(dot(L, N), 0.0) * MD;
  vec3 h = normalize(L + V);
  vec3 specular = MS * pow(clamp(dot(N, h), 0.0, 1.0), gamma) ;

  return clamp(diffuse + specular, 0.0, 1.0);
}

/* Function adapted from https://www.yaldex.com/open-gl/ch12lev1sec3.html and
 * https://www.shadertoy.com/view/lt2GRD (lines 25-57).
 *
 * If this was explained in the lecture, I must've missed it since in the slides
 * I found only 1 slide that uses only 3 values instead of 9
 */
vec3 ambient(vec3 N, vec3 MA) {
  const float C1 = 0.429043;
  const float C2 = 0.511664;
  const float C3 = 0.743125;
  const float C4 = 0.886227;
  const float C5 = 0.247708;

  vec3 la =  C1 * C22 * (N.x * N.x - N.y * N.y) +
              C3 * C20 * N.z * N.z +
              C4 * C00 -
              C5 * C20 +
              2.0 * C1 * C2m2 * N.x * N.y +
              2.0 * C1 * C21  * N.x * N.z +
              2.0 * C1 * C2m1 * N.y * N.z +
              2.0 * C2 * C11  * N.x +
              2.0 * C2 * C1m1 * N.y +
              2.0 * C2 * C10  * N.z;
  return la * MA;
}

void main() {
	vec3 N = normalize(fragNorm);               // surface normal
	vec3 V = normalize(gubo.eyePos - fragPos);  // viewer direction
	vec3 MD = texture(tex, fragUV).rgb;         // diffuse color
	vec3 MA = MD;                               // ambient color
	vec3 MS = vec3(1);                          // specular color
	vec3 ME = texture(texEmit, fragUV).rgb;     // emission color

	// Write the shader here
  vec3 unNormLx = gubo.lightPos - fragPos;
	vec3 lx = normalize(unNormLx);

  vec3 light = spotlight(unNormLx, lx);
  vec3 blinn = blinn(lx, N, V, MD, MS);
  vec3 ambient = ambient(N, MA);

  vec3 _out = clamp(ME + ambient + light * blinn, 0.0, 1.0);

	outColor = vec4(_out, 1.0f); // output color
}
