#include <cmath>

/// Add a translation matrix for the <n>-th piece with <(x,y,z)> translation
/// vector
void translate(Assignment02 *A, int n, float x, float y, float z) {
  // clang-format off
	A->Matrix(n,
			  1, 0, 0, x,
			  0, 1, 0, y,
			  0, 0, 1, z,
			  0, 0, 0, 1);
  // clang-format on
}

/// Add a scaling matrix for the <n>-th piece with <(x,y,z)> scaling vector
void scale(Assignment02 *A, int n, float x, float y, float z) {
  // clang-format off
	A->Matrix(n,
			  x, 0, 0, 0,
			  0, y, 0, 0,
			  0, 0, z, 0,
			  0, 0, 0, 1);
  // clang-format on
}

/// Convert degrees to radians
float deg2rad(float deg) { return (deg * M_PI) / 180; }

/// Add a rotation matrix of <deg> degrees around the z axis for the <n>-th
/// piece
void rotate_z(Assignment02 *A, int n, float deg) {
  float s = sin(deg2rad(deg));
  float c = cos(deg2rad(deg));
  // clang-format off
	A->Matrix(n,
			  c, -s, 0, 0,
			  s, c, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1);
  // clang-format on
}

/// Add a shear along the z axis for the <n>-th piece with the given parameters
void shear_z(Assignment02 *A, int n, float a, float b) {
  // clang-format off
	A->Matrix(n,
			  1, 0, a, 0,
			  0, 1, b, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1);
  // clang-format on
}

void SetupMatrices(Assignment02 *A) {
  /**************
   Method Matrix(int N, float v11, float v12, ... , float v14, float v21, ... ,
  float v44) of object A, receives in N the number of matrix that needs to be
  set, and in values v11 to v44 the 16 values composing the corrisponding matrix
  ***************/

  // To solve the puzzle: translate 2 along x, and 3 along z
  translate(A, 1, 2, 0, 3);

  // To solve the puzzle: rotate -15 degrees around the z axis
  rotate_z(A, 2, -15);

  // To solve the puzzle: mirror over the yz plane
  scale(A, 3, -1, 1, 1);

  // To solve the puzzle: perform a shear along the z axis, with hx = -0.5 (and
  // hy=0)
  shear_z(A, 4, -0.5, 0);

  // To solve the puzzle: scale of 2 along the x axis, and 1.25 on the z axis
  scale(A, 5, 2, 1, 1.25);

  // To solve the puzzle: perform a proportional scaling of a factor of 3
  scale(A, 6, 3, 3, 3);
}
