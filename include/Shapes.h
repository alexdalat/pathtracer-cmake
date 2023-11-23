#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Material.h"
#include "Object.h"
#include "Triangle.h"

inline std::vector<Object*> Rect(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                          glm::vec3 p4, Material mat) {
  std::vector<Object*> list;
  list.push_back(new Triangle(p1, p2, p3, mat));
  list.push_back(new Triangle(p3, p4, p1, mat));
  return list;
}

inline std::vector<Object*> RectPrism(glm::vec3 v1, glm::vec3 v2, Material mat) {
  std::vector<Object*> list;
  glm::vec3 v3 = glm::vec3(v1.x, v2.y, v1.z);
  glm::vec3 v4 = glm::vec3(v2.x, v1.y, v1.z);
  glm::vec3 v5 = glm::vec3(v1.x, v1.y, v2.z);
  glm::vec3 v6 = glm::vec3(v2.x, v2.y, v1.z);
  glm::vec3 v7 = glm::vec3(v1.x, v2.y, v2.z);
  glm::vec3 v8 = glm::vec3(v2.x, v1.y, v2.z);

  list.push_back(new Triangle(v1, v2, v3, mat));
  list.push_back(new Triangle(v2, v3, v4, mat));
  list.push_back(new Triangle(v1, v3, v5, mat));
  list.push_back(new Triangle(v3, v5, v7, mat));
  list.push_back(new Triangle(v1, v2, v5, mat));
  list.push_back(new Triangle(v2, v5, v6, mat));
  list.push_back(new Triangle(v2, v4, v6, mat));
  list.push_back(new Triangle(v4, v6, v8, mat));
  list.push_back(new Triangle(v3, v4, v7, mat));
  list.push_back(new Triangle(v4, v7, v8, mat));
  list.push_back(new Triangle(v5, v6, v7, mat));
  list.push_back(new Triangle(v6, v7, v8, mat));

  return list;
}
