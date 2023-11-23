#include "Sphere.h"

Sphere::Sphere(glm::vec3 const& position, float radius,
               Material const& material) {
  this->v1 = position;
  this->radius = radius;
  this->material = material;
}

float Sphere::calculateIntersection(Ray const& ray) {
  glm::vec3 oc = ray.origin - this->v1;
  float k1 = glm::dot(ray.direction, ray.direction);
  float k2 = 2 * glm::dot(oc, ray.direction);
  float k3 = glm::dot(oc, oc) - this->radius * this->radius;
  float discriminant = k2 * k2 - 4 * k1 * k3;
  if (discriminant < 0) {
    return -1;
  }
  float t1 = (-k2 - sqrt(discriminant)) / (2.0 * k1);
  float t2 = (-k2 + sqrt(discriminant)) / (2.0 * k1);
  if (t1 > 0.000000001) return t1;
  if (t2 > 0.000000001) return t2;
  return -1;
}

glm::vec3 Sphere::randomPoint() {
  return Ray::random_in_unit_sphere() * this->radius;
}

glm::vec3 Sphere::calculateNormal(glm::vec3 const& point,
                                  glm::vec3 const& dir) {
  return point - this->v1;  // should already be normalized
}

glm::vec3 Sphere::calculateUVCoordinates(glm::vec3 const& collisionPoint,
                                         glm::vec3 const& normal) {
  return glm::vec3(atan2(normal.x, normal.z) / (2.0f * M_PI) + 0.5f,
                   normal.y * 0.5f + 0.5f, 0.0f);
}
