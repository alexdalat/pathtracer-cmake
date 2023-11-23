#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Object.h"
#include "Ray.h"

class Triangle : public Object {
 public:

  Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3);
  Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3, Material const& material);

  float calculateIntersection(Ray const& ray) override;
  glm::vec3 calculateNormal(glm::vec3 const& point,
                            glm::vec3 const& dir) override;

 private:
  glm::vec3 edge1, edge2;
  glm::vec3 normal;
};
