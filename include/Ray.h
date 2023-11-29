#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "util.h"

class Ray {
 public:
  glm::vec3 origin;
  glm::vec3 direction;

  Ray(glm::vec3 origin, glm::vec3 direction)
      : origin(origin), direction(direction) {}

  static glm::vec3 random_in_unit_sphere();
  static glm::vec3 diffuse(glm::vec3 const& normal, glm::vec3 const& rand_vec);
  static glm::vec3 reflect(Ray const& ray, glm::vec3 const& normal);
};
