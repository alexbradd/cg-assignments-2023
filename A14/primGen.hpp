#include <glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0f)
#define i_mat(stride, x, y) ((x) * (stride) + (y))

using namespace glm;

void Assignment14::createCubeMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The primitive built here is a cube centered in the origin, whose edge lenght is 2.
  // Upper face
	vDef.push_back({{ 1.0f, 1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}});
	vDef.push_back({{-1.0f, 1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}});
	vDef.push_back({{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
	vDef.push_back({{ 1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);
	vIdx.push_back(0); vIdx.push_back(2); vIdx.push_back(3);
  // Lower face
	vDef.push_back({{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}});
	vDef.push_back({{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}});
	vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});
	vDef.push_back({{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}});
	vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);
	vIdx.push_back(4); vIdx.push_back(6); vIdx.push_back(7);
  // Right face
	vDef.push_back({{1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}});
	vDef.push_back({{1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}});
	vDef.push_back({{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});
	vDef.push_back({{1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});
	vIdx.push_back(8); vIdx.push_back(9); vIdx.push_back(10);
	vIdx.push_back(8); vIdx.push_back(10); vIdx.push_back(11);
  // Left face
	vDef.push_back({{-1.0f,  1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}});
	vDef.push_back({{-1.0f, -1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}});
	vDef.push_back({{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});
	vDef.push_back({{-1.0f,  1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}});
	vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(14);
	vIdx.push_back(12); vIdx.push_back(14); vIdx.push_back(15);
  // Front face
	vDef.push_back({{ 1.0f,  1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
	vDef.push_back({{-1.0f,  1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
	vDef.push_back({{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
	vDef.push_back({{ 1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
	vIdx.push_back(16); vIdx.push_back(17); vIdx.push_back(18);
	vIdx.push_back(16); vIdx.push_back(18); vIdx.push_back(19);
  // Back face
	vDef.push_back({{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});
	vDef.push_back({{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});
	vDef.push_back({{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});
	vDef.push_back({{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}});
	vIdx.push_back(20); vIdx.push_back(21); vIdx.push_back(22);
	vIdx.push_back(20); vIdx.push_back(22); vIdx.push_back(23);
}

void Assignment14::createFunctionMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.
  const int res = 40; // also stride

  // +1 so that we can work with the full [-3, +3] range
  const int res_corr = res + 1;

  float inc = 6.0f / ((float) res);
  float x_coord = -3.0f;
  float z_coord = -3.0f;
  for (int x = 0; x < res_corr; x++) {
    for (int z = 0; z < res_corr; z++) {
      float y = sin(x_coord) * cos(z_coord);
      vec3 du = vec3(1, cos(x_coord) * cos(z_coord), 0);
      vec3 dv = vec3(0, -sin(x_coord) * sin(z_coord), 1);
      vec3 n = - cross(du, dv);

      vDef.push_back({{x_coord, y, z_coord}, {n.x, n.y, n.z}});
      z_coord += inc;
    }
    x_coord += inc;
    z_coord = -3.0f;
  }

  // Matrix is in column major order with res_corr stride
  for (int x = 0; x < res_corr - 1; x++) {
    for (int z = 0; z < res_corr - 1; z++) {
      vIdx.push_back(i_mat(res_corr, x, z));
      vIdx.push_back(i_mat(res_corr, x, z + 1));
      vIdx.push_back(i_mat(res_corr, x + 1, z));

      vIdx.push_back(i_mat(res_corr, x, z + 1));
      vIdx.push_back(i_mat(res_corr, x + 1, z));
      vIdx.push_back(i_mat(res_corr, x + 1, z + 1));
    }
  }
}

void Assignment14::createCylinderMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.
  const int step = 12;
  const int n = 360 / step;

  vDef.push_back({{0.0f,  1.0f, 0.0f}, {0.0f,  1.0f, 0.0f}}); // 0
  vDef.push_back({{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}); // 1
  for (int i = 0; i < 360; i += step) {
    float angle = degToRad(i);
    float x = cos(angle);
    float z = sin(angle);
    vDef.push_back({{x,  1.0f, z}, {0.0f,  1.0f, 0.0f}});     // 2 + i*4 = {2, 6, ...}
    vDef.push_back({{x, -1.0f, z}, {0.0f, -1.0f, 0.0f}});     // 3 + i*4 = {3, 7, ...}
    vDef.push_back({{x,  1.0f, z}, {x, 0.0f, z}});            // 4 + i*4 = {4, 8, ...}
    vDef.push_back({{x, -1.0f, z}, {x, 0.0f, z}});            // 5 + i*4 = {5, 9, ...}
  }
  for (int i = 0; i < n; i++) {
    int next = i < n - 1 ? i + 1 : 0; // wrap around index
    vIdx.push_back(0); vIdx.push_back(2 + i * 4); vIdx.push_back(2 + next * 4);
    vIdx.push_back(1); vIdx.push_back(3 + i * 4); vIdx.push_back(3 + next * 4);
    vIdx.push_back(4 + i * 4); vIdx.push_back(5 + i * 4); vIdx.push_back(4 + next * 4);
    vIdx.push_back(5 + i * 4); vIdx.push_back(4 + next * 4); vIdx.push_back(5 + next * 4);
  }
}
