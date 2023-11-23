#pragma once

#include <glm/glm.hpp>

#include "util.h"

class Ray {
 public:
  glm::vec3 origin;
  glm::vec3 direction;

  Ray(glm::vec3 const& origin, glm::vec3 const& direction)
      : origin(origin), direction(direction) {}

  glm::vec3 across(float distance);
  static glm::vec3 random_in_unit_sphere();
  static glm::vec3 diffuse(glm::vec3 const& normal);
  static glm::vec3 reflect(Ray const& ray, glm::vec3 const& normal);
};
