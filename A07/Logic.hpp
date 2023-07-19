#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

/// Perspective projection parameters
typedef struct {
  float a;
  float n;
  float f;
  float fov_y_rad;
} norm_space_param;

/// Clamp <val> between <min> and <max>
float clamp(float val, float min, float max) {
  if (val < min)
    val = min;
  else if (val > max)
    val = max;
  return val;
}

/// Loops value <t> so that it is never larger than <length> or smaller than 0
///
/// Taken directly from the Unity reference source code
float repeat(float t, float length) {
  return clamp(t - floor(t / length) * length, 0.0f, length);
}

/// Calculates the shortest difference between two angles
///
/// Taken directly from the Unity reference source code
float delta_angle(float oldf, float newf) {
  float delta = repeat(newf - oldf, 2 * pi<float>());
  if (delta > pi<float>()) {
    delta -= 2 * pi<float>();
  }
  return delta;
}

/// Smoothly interpolates betweeen two given angles
float damp_angle(float oldf, float newf, float delta, float lambda = 15) {
  newf = oldf + delta_angle(oldf, newf);
  return oldf * exp(-lambda * delta) + newf * (1 - exp(-lambda * delta));
}

/// Camera object
///
/// Stores the basic properties of the camera (view parameters, position and
/// angles) and handles positioning w.r.t to the player's position
class Camera {
  const float rot_speed = radians(240.0f);
  const float height = 0.25f;
  const float distance = 1.5f;
  const float min_pitch = radians(-8.75f);
  const float max_pitch = radians(60.0f);

  vec3 pos;
  float yaw, pitch, roll;

public:
  Camera(vec3 initial_position = vec3(0)) {
    pos = initial_position;
    yaw = 0;
    pitch = 0;
    roll = pi<float>() / 2;
  }
  /// Updat angles and position based on the given player position
  void update_positioning(const vec3 &player_pos, const vec3 &r, float delta) {
    yaw = repeat(yaw - r.y * rot_speed * delta, 2 * pi<float>());
    roll = repeat(roll - r.z * rot_speed * delta, 2 * pi<float>());
    pitch = clamp(pitch + r.x * rot_speed * delta, -max_pitch, -min_pitch);

    pos = player_pos + quat(vec3(pitch, yaw, 0)) * vec3(0, height, distance);
  }
  vec3 get_pos() const { return pos; }
  float get_yaw() const { return yaw; }
  float get_pitch() const { return pitch; }
  float get_roll() const { return roll; }
  float get_height() const { return height; }
};

/// Player object
///
/// Stores the positioning and orientation of the player model and handles both
/// types of movement
class Amogus {
  const float move_speed = 2.0f;
  vec3 pos;

  // The model's yaw and the yaw used in computation are kepts separated to
  // avoid arced movement e.g. when changing movement direction by 180 degrees
  //
  // It has been done mainly for personal preference on 3rd person
  // controllers
  float axis_yaw;
  float model_yaw;

  bool strafing;

public:
  Amogus(vec3 starting_pos) {
    pos = starting_pos;
    axis_yaw = 0;
    model_yaw = 0;
    strafing = false;
  }
  /// Update character position depending on active strategy (strafe or not)
  /// w.r.t the current camera look direction
  void update_positioning(const Camera &camera, const vec3 &m, float delta) {
    vec3 movement =
        strafing ? strafe(m, camera, delta) : do_not_strafe(m, camera, delta);
    if (length(movement) > 1.0)
      movement = normalize(movement);

    pos += move_speed * delta * movement;
    pos.y += m.y * move_speed * delta;
  }
  void toggle_strafe_behaviour() {
    strafing = !strafing;
    std::cout << ">>> Toggled movement" << std::endl;
  }
  vec3 get_pos() const { return pos; }
  float get_yaw() const { return model_yaw; }

private:
  vec3 strafe(const vec3 &m, const Camera &camera, float delta) {
    if (m.z != 0.0 || m.x != 0.0) {
      axis_yaw = camera.get_yaw();
      model_yaw = axis_yaw;
    }

    vec3 x_local, z_local;
    compute_local_axes(x_local, z_local);
    return m.z * z_local - m.x * x_local;
  }
  vec3 do_not_strafe(const vec3 &m, const Camera &camera, float delta) {
    if (m.z != 0.0 || m.x != 0.0)
      axis_yaw = camera.get_yaw() + atan(-m.x, m.z);
    // model_yaw is always interpolated to avoid partial rotations when the
    // player releases control mid-rotation
    //
    // Again, personal preference; keeping them in sync changes nothing about
    // functionality
    model_yaw = damp_angle(model_yaw, axis_yaw, delta);

    vec3 x_local, z_local;
    compute_local_axes(x_local, z_local);
    return length(vec2(m.z, -m.x)) * z_local;
  }
  void compute_local_axes(vec3 &x_local, vec3 &z_local) {
    z_local = vec3(-sin(axis_yaw), 0, -cos(axis_yaw));
    x_local = normalize(cross(vec3(0, 1, 0), z_local));
  }
};

/// Create a perspective projection matrix using the given parameters
mat4 make_pm(const norm_space_param &c) {
  // clang-format off
  return mat4(1 / (c.a * tan(c.fov_y_rad)), 0,                    0,                         0,
              0,                           -1 / tan(c.fov_y_rad), 0,                         0,
              0,                            0,                    c.f / (c.n - c.f),        -1,
              0,                            0,                    (c.n * c.f) / (c.n - c.f), 0);
  // clang-format on
}

/// Create a look-at view matrix that follows the given player (both in angle
/// and position).
///
/// Roll is implemented by means of rotating the up-vector.
mat4 make_look_at(const Camera &camera, const Amogus &player) {
  vec3 target = player.get_pos();
  target.y += camera.get_height();

  vec3 vz = normalize(camera.get_pos() - target);
  vec3 vx = normalize(
      cross(vec3(cos(camera.get_roll()), sin(camera.get_roll()), 0), vz));
  vec3 vy = cross(vz, vx);
  mat4 Mc = mat4(vec4(vx, 0), vec4(vy, 0), vec4(vz, 0), vec4(0, 0, 0, 1));
  return inverse(Mc) * translate(mat4(1), -camera.get_pos());
}

/// Create the world matrix for the given player
mat4 make_wm(const Amogus &player) {
  return translate(mat4(1), player.get_pos()) *
         rotate(mat4(1), player.get_yaw(), vec3(0, 1, 0));
}

// The procedure must implement the game logic to move the character in third
// person.
//
// Inputs:
// - <Assignment07 *A> Pointer to the current assignment code
//   Required to read the input from the user
// - <float Ar> Aspect ratio of the current window (for the Projection Matrix)
//
// Output:
// - <glm::mat4 &ViewPrj> the view-projection matrix of the camera
// - <glm::mat4 &World> the world matrix of the object Parameters Camera FOV-y,
//   Near Plane and Far Plane
void GameLogic(Assignment07 *A, float Ar, mat4 &ViewPrj, mat4 &World) {
  const norm_space_param c{Ar, 0.1f, 100.0f, radians(45.0f / 2)};
  // Player starting point
  const vec3 StartingPosition = vec3(-41.50, 0.0, -19.5);

  // Integration with the timers and the controllers
  // returns:
  // - <float deltaT> the time passed since the last frame
  // - <vec3 m> the state of the motion axes of the controllers (-1 <= m.x,
  //   m.y, m.z <= 1)
  // - <vec3 r> the state of the rotation axes of the controllers
  //   (-1 <= r.x, r.y, r.z <= 1)
  // - <bool fire> if the user has pressed a fire button (not required in this
  //   assginment)
  float deltaT;
  vec3 m = vec3(0.0f), r = vec3(0.0f);
  bool fire = false;
  A->getSixAxis(deltaT, m, r, fire);

  // Game Logic implementation
  static Amogus player{StartingPosition};
  static Camera camera{StartingPosition};
  static bool prev_fire = false;

  camera.update_positioning(player.get_pos(), r, deltaT);

  if (fire != prev_fire) {
    player.toggle_strafe_behaviour();
  }
  prev_fire = fire;
  player.update_positioning(camera, m, deltaT);

  ViewPrj = make_pm(c) * make_look_at(camera, player);
  World = make_wm(player);
}
