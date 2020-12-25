#pragma once
#ifndef RAY_H
#define RAY_H

using namespace std;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 origin, glm::vec3 direction);
    glm::vec3 across(float distance);
    static glm::vec3 random_in_unit_sphere();
    static glm::vec3 diffuse(glm::vec3 normal);
    static glm::vec3 reflect(Ray ray, glm::vec3 normal);
};

#endif