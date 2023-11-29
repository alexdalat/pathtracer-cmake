#pragma once

#include <glm/glm.hpp>

#include "Ray.h"
#include "Rotation.h"

struct Controls {
  bool w = false;
  bool a = false;
  bool s = false;
  bool d = false;
  bool e = false;  // up
  bool q = false;  // down
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
};

class Camera {
 public:
  glm::vec3 position;
  Rotation rotation;
  glm::vec3 forward = glm::vec3(0, 0, -1);
  float aspectRatio;
  float fov;
  Controls controls;
  float moveSpeed = 0.1f;

  bool needUpdate = false;

  Camera(glm::vec3 position, float aspectRatio, float fov, Rotation rotation);

  glm::vec3 getRay(float const x, float const y);
  void lookAt(const glm::vec3 &target);

  void update(float deltaTime);

 private:
  float fov180;
  float fov180aspectRatio;
};
