
#include "Ray.h"

glm::vec3 Ray::across(float distance) {
  return origin + (direction * distance);
}

glm::vec3 Ray::random_in_unit_sphere() {
  glm::vec3 randomDirection = glm::normalize(glm::vec3({
      util::random() * 2.0f - 1.0f,
      util::random() * 2.0f - 1.0f,
      util::random() * 2.0f - 1.0f,
  })); // x = [-1, 1], y = [-1, 1], z = [-1, 1]
  return randomDirection;
}

glm::vec3 Ray::diffuse(glm::vec3 const& normal) {
  return normal + Ray::random_in_unit_sphere();
}

glm::vec3 Ray::reflect(Ray const& ray, glm::vec3 const& normal) {
  return ray.direction - (2.0f * glm::dot(ray.direction, normal) * normal);
}
