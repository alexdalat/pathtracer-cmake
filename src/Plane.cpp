#include "Plane.h"

Plane::Plane(glm::vec3 const& position, glm::vec3 const& normal,
             Material const& material) {
  this->v1 = position;
  this->normal = glm::normalize(normal);
  this->material = material;
  this->dnv1 = glm::dot(this->normal, this->v1);
}

float Plane::calculateIntersection(Ray const& ray) {
  float dnrd = glm::dot(this->normal, ray.direction);
  if (dnrd == 0) return -1;

  return (dnv1 - glm::dot(this->normal, ray.origin)) / dnrd;
}

glm::vec3 Plane::calculateNormal(glm::vec3 const& point, glm::vec3 const& dir) {
  return this->normal;
}
