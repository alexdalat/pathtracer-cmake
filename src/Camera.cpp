#include "Camera.h"

#include <glm/gtx/fast_square_root.hpp>  // fastNormalize. faster but less accurate

Camera::Camera(glm::vec3 position, float aspectRatio, float fov,
               Rotation rotation) {
  this->position = position;
  this->rotation = rotation;
  this->fov = fov;
  this->aspectRatio = aspectRatio;
}

Ray Camera::getRay(float x, float y) {
  glm::vec3 direction =
      glm::vec3((x - 0.5f) * (this->fov / 180.0f),
                (y - 0.5f) * (this->fov / 180.0f) * this->aspectRatio, 1.0f);

  direction = rotation.rotationMat * direction;
  // direction = direction * rotation.rollMat;
  // direction = direction * rotation.pitchMat;
  // direction = direction * rotation.yawMat;

  direction = glm::normalize(direction);

  return Ray(this->position, direction);
}

void Camera::lookAt(const glm::vec3 &target) {
  glm::vec3 direction = glm::normalize(target - this->position);
  this->rotation.pitch = asin(direction.y);
  this->rotation.yaw = -atan2(direction.x, direction.z) - M_PI;
  this->rotation.updatePitchMat();
  this->rotation.updateYawMat();
  this->rotation.updateRotationMat();
}

// movement functions using controls enum
void Camera::update(float deltaTime) {
  if (this->controls.w) {
    this->position += this->rotation.rotationMat * glm::vec3(0.0f, 0.0f, 1.0f) * moveSpeed;
  }
  if (this->controls.a) {
    this->position += this->rotation.rotationMat * glm::vec3(-1.0f, 0.0f, 0.0f) * moveSpeed;
  }
  if (this->controls.s) {
    this->position += this->rotation.rotationMat * glm::vec3(0.0f, 0.0f, -1.0f) * moveSpeed;
  }
  if (this->controls.d) {
    this->position += this->rotation.rotationMat * glm::vec3(1.0f, 0.0f, 0.0f) * moveSpeed;
  }
  if (this->controls.q) {
    this->position += this->rotation.rotationMat * glm::vec3(0.0f, -1.0f, 0.0f) * moveSpeed;
  }
  if (this->controls.e) {
    this->position += this->rotation.rotationMat * glm::vec3(0.0f, 1.0f, 0.0f) * moveSpeed;
  } 
}

