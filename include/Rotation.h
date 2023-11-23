#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>

class Rotation {
 public:
  float pitch;
  float yaw;
  float roll;
  glm::mat3 yawMat;
  glm::mat3 pitchMat;
  glm::mat3 rollMat;
  glm::mat3 rotationMat;

  Rotation();
  Rotation(float pitch, float yaw, float roll);

  void updatePitchMat();
  void updateYawMat();
  void updateRollMat();
  void updateRotationMat();
};
