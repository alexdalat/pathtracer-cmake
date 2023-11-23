#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Object.h"
#include "Ray.h"

class Plane : public Object {
 public:
  glm::vec3 normal;

  Plane(glm::vec3 const& position, glm::vec3 const& normal,
        Material const& material);

  float calculateIntersection(Ray const& ray) override;
  glm::vec3 calculateNormal(glm::vec3 const& point,
                            glm::vec3 const& dir) override;

 private:
  float dnv1;
};
