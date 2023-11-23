#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Ray.h"

// Object interface
class Object {
 public:
  glm::vec3 v1, v2, v3;
  Material material;
  // Texture texture;

  Object();
  Object(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3);
  Object(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3, Material const& material);

  virtual float calculateIntersection(Ray const& ray);
  virtual glm::vec3 calculateNormal(glm::vec3 const& point,
                                    glm::vec3 const& dir);
  virtual glm::vec3 randomPoint();
  virtual glm::vec3 calculateUVCoordinates(glm::vec3 const& collisionPoint,
                                           glm::vec3 const& normal);
};
