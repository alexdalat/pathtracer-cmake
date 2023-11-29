#include "Camera.h"

#include <glm/gtx/fast_square_root.hpp>  // fastNormalize. faster but less accurate

#include <iostream>

Camera::Camera(glm::vec3 position, float aspectRatio, float fov,
               Rotation rotation) {
  this->position = position;
  this->rotation = rotation;
  this->fov = fov;
  this->aspectRatio = aspectRatio;
  this->fov180 = fov / 180.0f;
  this->fov180aspectRatio = fov180 * aspectRatio;
}

glm::vec3 Camera::getRay(float const x, float const y) {
  /*
   * x: 0.0f -> 1.0f
   * y: 0.0f -> 1.0f
   */
  
  glm::vec3 direction = glm::vec3(
      (x * 2.0f - 1.0f) * fov180,
      (y * 2.0f - 1.0f) * fov180aspectRatio,
      1.0f
  );
  return this->rotation.rotationMat * direction;
}

void Camera::lookAt(const glm::vec3 &target) {
  glm::vec3 direction = glm::normalize(target - this->position);
  this->rotation.pitch = asin(direction.y);
  this->rotation.yaw = -atan2(direction.x, direction.z);
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

