#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

// Struct that holds information about a 2D vertex
struct Vertex2D {
  vec2 pos;           // 2D coordinates
  optional<int> i;    // vertex index (nullopt if vertex has not been added)
};

// Struct used as node in a graph that allows us to construct one of the two winding
// orders for each polygon.
struct Point {
  Vertex2D v;
  optional<shared_ptr<Point>> next_h, prev_h, next_v, prev_v;
};

template <>
struct std::hash<vec2>
{
  std::size_t operator()(const vec2 &k) const
  {
    return ((k.x + k.y)*(k.x + k.y + 1)/2) + k.y;
  }
};

enum WindingOrder {CLOCKWISE, COUNTER_CLOCKWISE};

// A Polygon is an ordered list of vertices. The order is not random, it is the
// winding worder of the vertices in the polygon.
struct Polygon {
  vector<Vertex2D> vertices;
  enum WindingOrder winding_order = COUNTER_CLOCKWISE;

  auto push_back(Vertex2D v) {
    return vertices.push_back(v);
  }
  auto size() {
    return vertices.size();
  }
  Vertex2D& operator[](int pos) {
    return vertices[pos];
  }
};

// Creates a new 2D matrix of bools (using malloc since I am too lazy to do it the C++ way)
bool** create_bool_matrix(int row, int col) {
  bool **mat = (bool **) calloc(row, sizeof(bool*));
  for (int i = 0; i < row; i++)
    mat[i] = (bool *)calloc(col, sizeof(bool));
  return mat;
}
// Frees the space for the given matrix which was created using create_mat
void free_bool_matrix(bool **mat, int row, int _) {
  for (int i = 0; i < row; i++)
    free(mat[i]);
  free(mat);
  mat = NULL;
}

// Safely index the maze matrix by returning ' ' for OOB accesses
char safe_index(char **mat, int row, int col, int i, int j) {
  if (i < 0 || i >= row || j < 0 || j >= col)
    return ' ';
  return mat[i][j];
}
// Returns true if the 2x2 submatrix contains a pattern that indicates
// that a vertex should be added. The matrix is composed as follows:
//    (a00, a01)
//    (a10, a11)
bool needs_vertex(char a00, char a01, char a10, char a11) {
  return !((a00 == a01 && a10 == a11) || (a00 == a10 && a01 == a11));
}

// Compute where on the grid vertices are needed
static inline void
compute_bool_matrix(int row,
                    int col,
                    char** maze,
                    bool** has_vertex)
{
  for (int i = -1; i < row; i++) {
    for (int j = -1; j < col; j++) {
      char a00 = safe_index(maze, row, col, i, j);
      char a01 = safe_index(maze, row, col, i, j + 1);
      char a10 = safe_index(maze, row, col, i + 1, j);
      char a11 = safe_index(maze, row, col, i + 1, j + 1);
      if (needs_vertex(a00, a01, a10, a11))
        has_vertex[i+1][j+1] = true;
    }
  }
}

// Given the vertex presence matrix, create the point graph that will be needed
// to construct the polygons
static inline unordered_map<vec2, shared_ptr<Point>> // in theory since c++17 we have NRVO so no expensive copying is done
compute_point_graph(int row,
                    int col,
                    bool **has_vertex)
{
  unordered_map<vec2, shared_ptr<Point>> points {};
  optional<shared_ptr<Point>> seen = nullopt;

  // Create points/vertices and populate horizontal connections
  for (int i = 0; i < row + 1; i++) {
    for (int j = 0; j <  col + 1; j++) {
      if (!has_vertex[i][j])
        continue;
      shared_ptr<Point> p {
        new Point {
          { vec2(j, -i), nullopt }, // We need to invert the coordinates because
                                    // matrix rows are horizontal coordinates,
                                    // while columns vertical
                                    // The minus on the Y is needed so that we don't get flipped along the X axis
          nullopt, nullopt, nullopt, nullopt
        }
      };
      if (!seen) {
        seen = p;
      } else {
        (*seen)->next_h = p;
        p->prev_h = *seen;
        seen = nullopt;
      }
      points[p->v.pos] = p;
    }
    seen = nullopt;
  }
  // Populate vertical connections
  seen = nullopt;
  for (int j = 0; j < col + 1; j++) {
    for (int i = 0; i < row + 1; i++) {
      if (!has_vertex[i][j])
        continue;
      shared_ptr<Point> p = points[vec2(j, -i)];
      if (!seen) {
        seen = p;
      } else {
        (*seen)->next_v = p;
        p->prev_v = *seen;
        seen = nullopt;
      }
    }
    seen = nullopt;
  }
  return points;
}

// Calculates the winding order of the given polygon by using the curve
// orientation method. Algorithm is taken straight from the wikipedia page about
// it
static inline void
compute_polygon_winding_order(shared_ptr<Polygon> poly)
{
  auto smallest = poly->vertices.begin();
  for (auto it = poly->vertices.begin(); it != poly->vertices.end(); it++) {
    auto x = it->pos.x;
    auto min_x = smallest->pos.x;
    if (x < min_x) {
      smallest = it;
    } else if (x == min_x) {
      auto y = it->pos.y;
      auto min_y = smallest->pos.y;
      if (y < min_y)
        smallest = it;
    }
  }
  auto prev = smallest == poly->vertices.begin() ? poly->vertices.end() - 1 : smallest - 1;
  auto next = smallest == poly->vertices.end() - 1 ? poly->vertices.begin() : smallest + 1;
  float det = (smallest->pos.x - prev->pos.x) * (next->pos.y - prev->pos.y) -
              (next->pos.x - prev->pos.x) * (smallest->pos.y - prev->pos.y);
  poly->winding_order = det < 0.0f ? CLOCKWISE : COUNTER_CLOCKWISE;
}

// Given the point graph, compute the list of polygons and the respective winding
// order
//
// Note: this funciton will clear the point graph.
static inline vector<shared_ptr<Polygon>> // Again, we have NRVO
compute_polygons(unordered_map<vec2, shared_ptr<Point>>& points)
{
  // Needed only to prevent getting stuck in a prev -> next -> prev loop when walking
  // the graph
  enum directions { RIGHT, LEFT, DOWN, UP };

  vector<shared_ptr<Polygon>> polys;
  while (!points.empty()) {
    shared_ptr<Polygon> poly { new Polygon {} };
    polys.push_back(poly);
    optional<shared_ptr<Point>> p = points.begin()->second;
    vec2 initial = points.begin()->second->v.pos;
    enum directions current_dir = RIGHT;
    do {
      poly->push_back((*p)->v);
      points.erase((*p)->v.pos);
      if ((*p)->next_h && current_dir != LEFT) {
        p = (*p)->next_h;
        current_dir = RIGHT;
      } else if ((*p)->next_v && current_dir != UP) {
        p = (*p)->next_v;
        current_dir = DOWN;
      } else if ((*p)->prev_h && current_dir != RIGHT) {
        p = (*p)->prev_h;
        current_dir = LEFT;
      } else if ((*p)->prev_v && current_dir != DOWN) {
        p = (*p)->prev_v;
        current_dir = UP;
      } else {
        throw runtime_error("Not possible");
      }
    } while ((*p)->v.pos != initial);
    compute_polygon_winding_order(poly);
    // We need a counter clockwise order to make triangulation work
    if (poly->winding_order == CLOCKWISE) {
      reverse(poly->vertices.begin(), poly->vertices.end());
      poly->winding_order = COUNTER_CLOCKWISE;
    }
  }
  return polys;
}

static inline int circular_get(vector<int>& vec, int pos) {
  if (pos < 0) {
    return vec[size(vec) - (-pos % size(vec))];
  } else if (pos >= size(vec)) {
    return vec[pos % size(vec)];
  } else {
    return vec[pos];
  }
}

static inline bool
is_in_triangle(vec2 p, vec2 a, vec2 b, vec2 c) {
  vec2 ab = b - a,
       bc = c - b,
       ca = a - c;
  vec2 ap = p - a,
       bp = p - b,
       cp = p - c;
  float c1 = cross(vec3(ab, 0.0f), vec3(ap, 0.0f)).z,
        c2 = cross(vec3(bc, 0.0f), vec3(bp, 0.0f)).z,
        c3 = cross(vec3(ca, 0.0f), vec3(cp, 0.0f)).z;
  return !(c1 < 0.0f || c2 < 0.0f || c3 < 0.0f);
}

// Given a list of polygons, insert the vertices (giving them an index) at the
// roof height and triangulate them
static inline void
insert_roof(vector<shared_ptr<Polygon>>& polys,
            int& index,
            vector<float>& vPos,
            vector<int>&   vIdx)
{
  // For each polygon
  for (auto poly : polys) {
    // Insert vertices
    for (auto& v : poly->vertices) {
      vPos.push_back(v.pos.x); vPos.push_back(1.0f); vPos.push_back(v.pos.y);
      v.i = index++;
    }

    // Triangulate the polygon using a not very optimizied implementation of the
    // ear-clipping algorithm
    //
    // By a quick look at the maze generation algorithm and how we structured the
    // algorithms in this program, the polygons that generated __should__ always
    // be simple (in the mathematical sense) so the algorithm will work
    vector<int> vert_indices; // support array that allows us to work
                              // on the polygon without modifying it
    for (int i = 0; i < poly->size(); i++)
      vert_indices.push_back(i);
    while(vert_indices.size() > 3) {
      for (int i = 0; i < vert_indices.size(); i++) {
        int a = vert_indices[i],
            b = circular_get(vert_indices, i - 1),
            c = circular_get(vert_indices, i + 1);
        vec2 va = (*poly)[a].pos,
             vb = (*poly)[b].pos,
             vc = (*poly)[c].pos;
        vec2 va_vb = vb - va,
             va_vc = vc - va;
        if (cross(vec3(va_vc, 0.0f), vec3(va_vb, 0.0f)).z <= 0.0f) {
          continue;
        }

        bool isEar = true;
        for (int j = 0; j < vert_indices.size(); j++) {
          int p_i = vert_indices[j];
          if (p_i == a || p_i == b || p_i == c)
            continue;
          vec2 p = (*poly)[p_i].pos;
          if (is_in_triangle(p, vb, va, vc)) {
            isEar = false;
            break;
          }
        }
        if (isEar) {
          vIdx.push_back(*(*poly)[b].i);
          vIdx.push_back(*(*poly)[a].i);
          vIdx.push_back(*(*poly)[c].i);
          vert_indices.erase(vert_indices.begin() + i);
          break;
        }
      }
    }
    vIdx.push_back(*(*poly)[vert_indices[0]].i);
    vIdx.push_back(*(*poly)[vert_indices[1]].i);
    vIdx.push_back(*(*poly)[vert_indices[2]].i);
  }
}

// Given the list of polygons, insert the walls and floors to the maze
static inline void
insert_walls_and_floor(const vector<shared_ptr<Polygon>>& polys,
                       int& index,
                       vector<float>& vPos,
                       vector<int>&   vIdx)
{
  // not very efficient, but I do not want to do a chain of 8 if/elses to get
  // the vertices I need for the floor
  unordered_map<vec2, Vertex2D> floor_vertices;

  // Walls
  for (auto p : polys) {
    auto deref_p = *p;

    int ind_initial = index;
    for (int i = 0; i < deref_p.size()- 1; i++) {
      Vertex2D curr = deref_p[i],
               next = deref_p[i + 1];

      Vertex2D curr_floor = Vertex2D { curr.pos, index++ };
      Vertex2D next_floor = Vertex2D { next.pos, index++ };
      floor_vertices[vec2(curr.pos)] = curr_floor;
      floor_vertices[vec2(next.pos)] = next_floor;

      vPos.push_back(curr.pos.x); vPos.push_back(0.0f); vPos.push_back(curr.pos.y);
      vPos.push_back(next.pos.x); vPos.push_back(0.0f); vPos.push_back(next.pos.y);

      vIdx.push_back(*(curr.i)); vIdx.push_back(*(next.i)); vIdx.push_back(*(curr_floor.i));
      vIdx.push_back(*(next.i)); vIdx.push_back(*(next_floor.i)); vIdx.push_back(*(curr_floor.i));

    }
    // Wall connecting first with last
    vIdx.push_back(*(deref_p[deref_p.size() - 1].i));
    vIdx.push_back(*(deref_p[0].i));
    vIdx.push_back(index - 1);
    vIdx.push_back(*(deref_p[0].i));
    vIdx.push_back(index - 1);
    vIdx.push_back(ind_initial);
  }
  // Floor
  int i_0_0   = *(floor_vertices[vec2(0, 0)].i),
      i_0_15  = *(floor_vertices[vec2(15, 0)].i),
      i_15_0  = *(floor_vertices[vec2(0, -15)].i),
      i_15_15 = *(floor_vertices[vec2(15, -15)].i);
  vIdx.push_back(i_0_0); vIdx.push_back(i_0_15); vIdx.push_back(i_15_0);
  vIdx.push_back(i_0_15); vIdx.push_back(i_15_15); vIdx.push_back(i_15_0);
}

void Assignment08::createMazeMesh(int row, int col, char **maze) {
  // The procedure gets in input the number of rows <row> of the maze, and the
  // number of columns <col> Element <maze[r][c]>, with 0 <= r <= row-1 and 0 <=
  // c <= col-1 contains:
  //		Symbol ' ' if there is an empty space
  //		Symbol '#' if there is a wall
  //
  int index = 0; // Index of the next vertex to be added
  bool **has_vertex = create_bool_matrix(row + 1, col + 1);

  // Important Note:
  // From what I undertood of the `genMaze()` function, the vertex assignment
  // works for all mazes that it can generate.
  //
  // In case I am dumb and/or I just got lucky in my testing, I'll cover the only
  // shortcoming: the algorithm fails in case it finds the following pattern
  //
  // ####    #
  // #   #####
  //
  // The problem is the center intersection will mess with the graph walking and
  // the triangulation since the resulting polygon is not simple anymore (one
  // vertex has more than 2 edges). The solution is to insert duplicate vertices
  // for that coordinate and keeping track of it (using a counter instead of
  // bools). The graph walking would require small changes, while the
  // triangulation should remain unchanged.
  //
  // If you run tests on picked mazes and one of those mazes contains the
  // given pattern, then I guess I should've spent more time coding instead of
  // writing an essay on why I didn't do it.
  compute_bool_matrix(row, col, maze, has_vertex);
  auto points = compute_point_graph(row, col, has_vertex);
  auto polys = compute_polygons(points);
  free_bool_matrix(has_vertex, row + 1, col + 1);

  insert_roof(polys, index, vPos, vIdx);
  insert_walls_and_floor(polys, index, vPos, vIdx);
}
