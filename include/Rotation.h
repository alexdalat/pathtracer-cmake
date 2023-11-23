#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>

class Rotation {
 public:
  float pitch;
  float yaw;
  float roll;
  glm::lowp_f32mat3 yawMat;
  glm::lowp_f32mat3 pitchMat;
  glm::lowp_f32mat3 rollMat;
  glm::lowp_f32mat3 rotationMat;

  Rotation();
  Rotation(float pitch, float yaw, float roll);

  void updatePitchMat();
  void updateYawMat();
  void updateRollMat();
  void updateRotationMat();
};
