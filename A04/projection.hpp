#include <cmath>
#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

using namespace glm;

/// Given a parallel projection matrix, compute an axonometric projection matrix
/// qith the given parameters
mat4 axonometric(mat4 model, float y_deg, float x_deg) {
  float y_rad{radians(y_deg)}, x_rad{radians(x_deg)};

  return rotate(model, x_rad, vec3(1, 0, 0)) *
         rotate(mat4(1), y_rad, vec3(0, 1, 0));
}

/// Given a parallel projection matrix, compute an oblique projection matrix
/// qith the given parameters
mat4 oblique(mat4 model, float z_deg, float rho) {
  float z_rad{radians(z_deg)};

  return shearZ3D(model, -rho * std::cos(z_rad), -rho * std::sin(z_rad));
}

void SetupProjectionMatrices(Assignment04 *A, float Ar) {
  /**************
   Method Matrix(int N, glm::mat4 M) of object A,
   receives in N the number of the matrix that needs to be set, and the matrix
  in M
  ***************/

  constexpr float w{10.0f}, n{-50.0f}, f{50.0f};
  // clang-format off
  const mat4 parallel_proj_m = mat4(1 / w, 0,      0,           0,
                                    0,    -Ar / w, 0,           0,
                                    0,     0,      1 / (n - f), 0,
                                    0,     0,      n / (n - f), 1);
  // clang-format on

  // Isometric projection
  A->Matrix(1, axonometric(parallel_proj_m, -45.0f, 35.26f));
  // Dimetric
  A->Matrix(2, axonometric(parallel_proj_m, -45.0f, 20.0f));
  // Trimetric
  A->Matrix(3, axonometric(parallel_proj_m, -60.0f, 20.0f));
  // Cabinet
  A->Matrix(4, oblique(parallel_proj_m, 45.0f, 0.5f));
}
