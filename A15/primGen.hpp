#define _USE_MATH_DEFINES
#include <cmath>

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0f)
#define i_mat(stride, x, y) ((x) * (stride) + (y))

void Assignment15::createBoxMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a box centered in the origin, with proportions respecting the texture.

  // Upper face
	vDef.push_back({{ 0.28f, 1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.09f,  0.18f}});
	vDef.push_back({{-0.28f, 1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.09f,  0.0f }});
	vDef.push_back({{-0.28f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.435f, 0.0f }});
	vDef.push_back({{ 0.28f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.435f, 0.18f}});
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);
	vIdx.push_back(0); vIdx.push_back(2); vIdx.push_back(3);
  // Lower face
	vDef.push_back({{ 0.28f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}, {0.09f,  1.0f }});
	vDef.push_back({{-0.28f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}, {0.09f,  0.82f}});
	vDef.push_back({{-0.28f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.435f, 0.82f}});
	vDef.push_back({{ 0.28f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.435f, 1.0f }});
	vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);
	vIdx.push_back(4); vIdx.push_back(6); vIdx.push_back(7);
  // Right face
	vDef.push_back({{0.28f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.09f,  0.18f}});
	vDef.push_back({{0.28f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.09f,  0.82f}});
	vDef.push_back({{0.28f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.435f, 0.82f}});
	vDef.push_back({{0.28f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.435f, 0.18f}});
	vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);
	vIdx.push_back(8); vIdx.push_back(10); vIdx.push_back(11);
  // Left face
	vDef.push_back({{-0.28f,  1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}, {0.865f, 0.18f}});
	vDef.push_back({{-0.28f, -1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}, {0.865f, 0.82f}});
	vDef.push_back({{-0.28f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.525f, 0.82f}});
	vDef.push_back({{-0.28f,  1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.525f, 0.18f}});
	vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);
	vIdx.push_back(12); vIdx.push_back(14); vIdx.push_back(15);
  // Front face
	vDef.push_back({{ 0.28f,  1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.09f, 0.18f}});
	vDef.push_back({{-0.28f,  1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f,  0.18f}});
	vDef.push_back({{-0.28f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f,  0.82f}});
	vDef.push_back({{ 0.28f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.09f, 0.82f}});
	vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);
	vIdx.push_back(16); vIdx.push_back(18); vIdx.push_back(19);
  // Back face
	vDef.push_back({{ 0.28f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.435f, 0.18f}});
	vDef.push_back({{-0.28f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.525f, 0.18f}});
	vDef.push_back({{-0.28f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.525f, 0.82f}});
	vDef.push_back({{ 0.28f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.435f, 0.82f}});
	vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);
	vIdx.push_back(20); vIdx.push_back(22); vIdx.push_back(23);
}

void Assignment15::createSphereMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a sphere of radius 1, centered in the origin, on which the Mars texture is applied seamless.
  const int n_slices = 40; // meridians
  const int n_rings  = 40; // parallels

  // +1 because we want:
  // 1. [0;360] because we want the duplicate of the first slice for the texture wrap
  // 2. [0; 180] because thats how spherical coordinates work and we need duplicate
  //    vertices for both the bottom and top vertex so that we can correctly wrap the texture
  const int n_slices_corr = n_slices + 1;
  const int n_rings_corr = n_rings + 1;

  const float inc_step = 180.0f / ((float) n_rings);
  const float azi_step = 360.0f / ((float) n_slices);
  const float u_step = 1 / ((float) n_slices);
  const float v_step = 1 / ((float) n_rings);

  float inc = 0.0f;
  float azi = 0.0f;
  float u = 0.0f;
  float v = 0.0f;

  for (int i = 0; i < n_slices_corr; i++) {
    for (int j = 0; j < n_rings_corr; j++) {
      float inc_rad = degToRad(inc);
      float azi_rad = degToRad(azi);
      // Using sperical coordinates we get:
      Vertex vert {
        {sin(inc_rad) * cos(azi_rad), cos(inc_rad), sin(inc_rad) * sin(azi_rad)},
        {sin(inc_rad) * cos(azi_rad), cos(inc_rad), sin(inc_rad) * sin(azi_rad)},
        {u, v},
      };
      vDef.push_back(vert);
      inc += inc_step;
      v += v_step;
    }
    azi += azi_step;
    u += u_step;
    inc = 0.0f;
    v = 0.0f;
  }
  //
  // Matrix is in column major order with n_rings_corr stride
  for (int i = 0; i < n_slices_corr - 1; i++) {
    // Top "cap"
    vIdx.push_back(i_mat(n_rings_corr, i,     0));
    vIdx.push_back(i_mat(n_rings_corr, i,     1));
    vIdx.push_back(i_mat(n_rings_corr, i + 1, 1));
    // Bottom "cap"
    vIdx.push_back(i_mat(n_rings_corr, i,     n_rings_corr - 2));
    vIdx.push_back(i_mat(n_rings_corr, i + 1, n_rings_corr - 2));
    vIdx.push_back(i_mat(n_rings_corr, i,     n_rings_corr - 1));

    for (int j = 1; j < n_rings_corr - 2; j++) {
      vIdx.push_back(i_mat(n_rings_corr, i,     j));
      vIdx.push_back(i_mat(n_rings_corr, i,     j + 1));
      vIdx.push_back(i_mat(n_rings_corr, i + 1, j));

      vIdx.push_back(i_mat(n_rings_corr, i,     j + 1));
      vIdx.push_back(i_mat(n_rings_corr, i + 1, j));
      vIdx.push_back(i_mat(n_rings_corr, i + 1, j + 1));
    }
  }
}

