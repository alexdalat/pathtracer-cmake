#pragma once

class Plane : public Object {
public:
    glm::vec3 position;
    glm::vec3 normal;
    Material material;

    Plane();

    Plane(glm::vec3 position, glm::vec3 normal, Material material);

    float calculateIntersection(Ray *ray);

    glm::vec3 calculateNormal(glm::vec3 &point, glm::vec3 &dir);
    Material getMaterial();
};
