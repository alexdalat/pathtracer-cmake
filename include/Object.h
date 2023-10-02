#pragma once

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
