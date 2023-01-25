#pragma once
#ifndef OBJECT_H
#define OBJECT_H

using namespace std;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Ray.h"
#include "Material.h"
#include "Color.h"
#include <string>

// Object interface
class Object {
public:
    Object();

    virtual float calculateIntersection(Ray *ray);

    virtual glm::vec3 calculateNormal(glm::vec3 &point, glm::vec3 &dir);
    virtual glm::vec3 randomPoint();
    virtual Material getMaterial();
    virtual glm::vec3 calculateUVCoordinates(glm::vec3 collisionPoint, glm::vec3 normal);
};

#endif