//
// Created by xelada on 2/9/20.
//

#include "Plane.h"
#include <iostream>

Plane::Plane() {
    normal = glm::vec3();
    material = Material();
}

Plane::Plane(glm::vec3 position, glm::vec3 normal, Material material) {
    this->position = position;
    this->normal = normal;
    this->material = material;
}

float Plane::calculateIntersection(Ray *ray) {
    if (glm::dot(this->normal, glm::normalize(ray->direction)) == 0) {
        return -1;
    }
    float t = (glm::dot(this->normal, this->position) - glm::dot(this->normal, ray->origin)) /
              glm::dot(this->normal, glm::normalize(ray->direction));
    return t;
}

glm::vec3 Plane::calculateNormal(glm::vec3 &point, glm::vec3 &dir) {
    return this->normal;
}

Material Plane::getMaterial() {
    return material;
}
