#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H

using namespace std;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Object.h"
#include "Color.h"
#include "Material.h"
#include "Ray.h"

class Triangle : public Object {
public:
    // glm::vec3 position; // from Object
    glm::vec3 v1, v2, v3;
    Material material;
    Color color;
    //Texture texture;

    Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Material material);
    float calculateIntersection(Ray *ray);
    glm::vec3 calculateNormal(glm::vec3 &point, glm::vec3 &dir);
    Material getMaterial();
};

#endif