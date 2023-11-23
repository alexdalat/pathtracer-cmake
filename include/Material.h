#pragma once

#include <glm/glm.hpp>

class Material {
 public:
  float reflectivity;
  float diffuse;
  float emissive;
  glm::vec3 color;

  Material();

  Material(float reflectivity, float diffuse, float emissive, glm::vec3 color);
};
