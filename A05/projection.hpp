#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

typedef struct {
  float a;
  float n;
  float f;
} norm_space_consts;

/// Given the normalized space paramenters and the vertical FOV in degrees,
/// compute a perspective projection matrix
mat4 perspective_fov_y(const norm_space_consts *c, float fov_y) {
  float fov_y_rad{radians(fov_y / 2)}, a{c->a}, f{c->f}, n{c->n};
  // clang-format off
  return mat4(1 / (a * tan(fov_y_rad)), 0,                  0,                 0,
              0,                       -1 / tan(fov_y_rad), 0,                 0,
              0,                        0,                  f / (n - f),      -1,
              0,                        0,                  (n * f) / (n - f), 0);
  // clang-format on
}

/// Given the normalized space paramenters and the horizontal FOV in degrees,
/// compute a perspective projection matrix
mat4 perspective_fov_x(const norm_space_consts *c, float fov_x) {
  float fov_x_rad{radians(fov_x / 2)}, a{c->a}, f{c->f}, n{c->n};
  // clang-format off
  return mat4(1 / tan(fov_x_rad), 0,                  0,                 0,
              0,                 -a / tan(fov_x_rad), 0,                 0,
              0,                  0,                  f / (n - f),      -1,
              0,                  0,                  (n * f) / (n - f), 0);
  // clang-format on
}

void SetupProjectionMatrices(Assignment05 *A, float Ar) {
  /**************
  Method Matrix(int N, glm::mat4 M) of object A,
  receives in N the number of the matrix that needs to be set, and the matrix
  in M
  ***************/

  const norm_space_consts c{Ar, 0.1f, 50.0f};

  // Fov-y = 60 deg
  A->Matrix(1, perspective_fov_y(&c, 60.0f));
  // Fov-y = 105 deg
  A->Matrix(2, perspective_fov_y(&c, 105.0f));
  // Fov-y = 25 deg
  A->Matrix(3, perspective_fov_y(&c, 25.0f));
  // Right View, Fov-x = 45 deg
  A->Matrix(4, perspective_fov_x(&c, 45.0f));
}
