#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Object.h"
#include "Ray.h"

class Sphere : public Object {
 public:
  // inherits material and color from Object
  float radius;

  Sphere(glm::vec3 const& position, float radius, Material const& material);

  float calculateIntersection(Ray const& ray) override;
  glm::vec3 randomPoint() override;
  glm::vec3 calculateNormal(glm::vec3 const& point,
                            glm::vec3 const& dir) override;
  glm::vec3 calculateUVCoordinates(glm::vec3 const& collisionPoint,
                                   glm::vec3 const& normal) override;
};
