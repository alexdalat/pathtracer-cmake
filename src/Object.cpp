#include "Object.h"

#include <stdexcept>

Object::Object() {
  this->v1 = glm::vec3(0.0f);
  this->v2 = glm::vec3(0.0f);
  this->v3 = glm::vec3(0.0f);
  this->material = Material();
}

Object::Object(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3) {
  this->v1 = v1;
  this->v2 = v2;
  this->v3 = v3;
  this->material = Material();
}

Object::Object(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3,
               Material const& material) {
  Object(v1, v2, v3);
  this->material = material;
}

float Object::calculateIntersection(Ray const& ray) {
  throw std::runtime_error("Object::calculateIntersection() called");
  return -1;
}

glm::vec3 Object::randomPoint() {
  throw std::runtime_error("Object::randomPoint() called");
  return glm::vec3(0.0f);
}

glm::vec3 Object::calculateNormal(glm::vec3 const& point,
                                  glm::vec3 const& dir) {
  throw std::runtime_error("Object::calculateNormal() called");
  return glm::vec3(0.0f);
}

glm::vec3 Object::calculateUVCoordinates(glm::vec3 const& collisionPoint,
                                         glm::vec3 const& normal) {
  throw std::runtime_error("Object::calculateUVCoordinates() called");
  return glm::vec3(0.0f); 
}
