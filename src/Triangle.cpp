#include "Triangle.h"

Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3) {
  this->v1 = v1; this->v2 = v2; this->v3 = v3;
  this->edge1 = this->v2 - this->v1;
  this->edge2 = this->v3 - this->v1;
  this->normal = glm::normalize(glm::cross(this->edge1, this->edge2));
}

Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3,
                   Material const& material)
    : Triangle(v1, v2, v3) {
  this->material = material;
}

float Triangle::calculateIntersection(
    Ray const& ray) {  // Möller–Trumbore intersection algorithm
  glm::vec3 pvec = glm::cross(ray.direction, this->edge2);
  float det = glm::dot(this->edge1, pvec);
  if (det > -0.000001 && det < 0.000001) return -1;
  float invDet = 1.0f / det;
  glm::vec3 tvec = ray.origin - this->v1;
  float u = glm::dot(tvec, pvec) * invDet;
  if (u < 0 || u > 1) return -1;
  glm::vec3 qvec = glm::cross(tvec, this->edge1);
  float v = glm::dot(ray.direction, qvec) * invDet;
  if (v < 0 || u + v > 1) return -1;
  return glm::dot(this->edge2, qvec) * invDet;  // return distance
}

glm::vec3 Triangle::calculateNormal(glm::vec3 const& point,
                                    glm::vec3 const& dir) {
  if (glm::dot(dir, normal) < 0)
    return this->normal;
  else
    return -this->normal;
}
