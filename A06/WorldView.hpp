#include "glm/ext/matrix_transform.hpp"
#include <cmath>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

/// Create the projection matrix with:
/// 1. near plane at 0.1
/// 2. far plane at 50.0
/// 3. aspect ratio <a>
/// 4: vertical FOV of 60 degrees
mat4 projection_matrix(float a) {
  float n{0.1f}, f{50.0f}, tan_fov{static_cast<float>(sqrt(3.0f)) / 3};
  // clang-format off
  return mat4(1 / (a * tan_fov), 0,           0,                 0,
              0,                -1 / tan_fov, 0,                 0,
              0,                 0,           f / (n - f),      -1,
              0,                 0,           (n * f) / (n - f), 0);
  // clang-format on
}

/// Create the view matrix using the given parameters
mat4 view_matrix(float alpha, float beta, float rho, vec3 pos) {
  return rotate(mat4(1), -rho, vec3(0, 0, 1)) *
         rotate(mat4(1), -beta, vec3(1, 0, 0)) *
         rotate(mat4(1), -alpha, vec3(0, 1, 0)) * translate(mat4(1), -pos);
}

/// Create a View-Projection Matrix, with near plane at 0.1, and far plane
/// at 50.0, and aspect ratio given in Ar. The view matrix, uses the
/// Look-in-Direction model, with vector Pos specifying the position of the
/// camera, and angles Alpha, Beta and Rho defining its direction. In
/// particular, Alpha defines the direction (Yaw), Beta the elevation (Pitch),
/// and Rho the roll.
mat4 MakeViewProjectionMatrix(float Ar, float Alpha, float Beta, float Rho,
                              vec3 Pos) {
  return projection_matrix(Ar) * view_matrix(Alpha, Beta, Rho, Pos);
}

/// Creates and returns a World Matrix that positions the object at <pos>,
/// orients it according to <rQ>, and scales it according to the sizes
/// given in vector <size>
mat4 MakeWorldMatrix(vec3 pos, quat rQ, vec3 size) {
  return translate(mat4(1), pos) * mat4(rQ) * scale(mat4(1), size);
}
