#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

void SetupMatrices(Assignment03 *A) {
  /**************
  Method Matrix(int N, glm::mat4 M) of object A,
  receives in N the number of the matrix that needs to be set, and the matrix
  in M
  ***************/

  mat4 S;

  // To solve the puzzle: scale 2x proportionally, centered in (2,0,-3)
  S = translate(mat4(1), vec3(2, 0, -3)) *
      scale(mat4(1), vec3(2)) *
      translate(mat4(1), vec3(-2, 0, 3));
  A->Matrix(1, S); // sets the matrix corresponding to piece 1

  // To solve the puzzle: scale 3x on an axis oriented 45 degrees between the x
  // and z axis and passing through point (1,0,0)
  S = translate(mat4(1), vec3(1, 0, 0)) *
      rotate(mat4(1), radians(45.0f), vec3(0, 1, 0)) *
      scale(mat4(1), vec3(3, 1, 1)) *
      rotate(mat4(1), radians(-45.0f), vec3(0, 1, 0)) *
      translate(mat4(1), vec3(-1, 0, 0));
  A->Matrix(2, S); // sets the matrix corresponding to piece 2

  // To solve the puzzle: find it yourself! Hint: compose a rotation around an
  // arbitrary point with a translation
  S = translate(mat4(1), vec3(1, 0, 2)) *
      rotate(mat4(1), radians(45.0f), vec3(0, 1, 0)) *
      translate(mat4(1), vec3(-2, 0, -3));
  A->Matrix(3, S); // sets the matrix corresponding to piece 3

  // To solve the puzzle: rotate 60 degree around the y axis, centered in
  // (-1,0,-2)
  S = translate(mat4(1), vec3(-1, 0, -2)) *
      rotate(mat4(1), radians(60.0f), vec3(0, 1, 0)) *
      translate(mat4(1), vec3(1, 0, 2));
  A->Matrix(4, S); // sets the matrix corresponding to piece 4

  // To solve the puzzle: rotate -90 degree around an arbitrary axis passing in
  // (-1,0,0). The x axis can be aligned to this arbitrary direction with a
  // rotation of -45 around the y axis.
  S = translate(mat4(1), vec3(-1, 0, 0)) *
      rotate(mat4(1), radians(-90.0f), normalize(vec3(1, 0, 1))) *
      translate(mat4(1), vec3(1, 0, 0));
  A->Matrix(5, S); // sets the matrix corresponding to piece 5

  // To solve the puzzle: find it yourself! Hint: compose a non-proportional
  // scaling that halves the object along one of the main axes, centered in an
  // arbitrary point, with a translation
  S =
      translate(mat4(1), vec3(-1, 0, 2)) *
      scale(mat4(1), vec3(0.5f, 1.0f, 1.0f)) *
      translate(mat4(1), vec3(3, 0, -2));
  A->Matrix(6, S); // sets the matrix corresponding to piece 6
}
