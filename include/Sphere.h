#pragma once

class Sphere : public Object {
public:
    glm::vec3 position;
    float radius;
    Material material;

    Sphere();
    Sphere(glm::vec3 position, float radius, Material material);

    float calculateIntersection(Ray *ray);
    glm::vec3 randomPoint();
    glm::vec3 calculateNormal(glm::vec3 &point, glm::vec3 &dir);
    Material getMaterial();
    glm::vec3 calculateUVCoordinates(glm::vec3 collisionPoint, glm::vec3 normal);
};
