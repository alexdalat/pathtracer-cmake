
#include "Sphere.h"

Sphere::Sphere() {
    this->position = glm::vec3();
    this->radius = 0;
    this->material = Material();
}

Sphere::Sphere(glm::vec3 position, float radius, Material material) {
    this->position = position;
    this->radius = radius;
    this->material = material;
}

float Sphere::calculateIntersection(Ray *ray) {
    glm::vec3 oc = ray->origin - this->position;
    float k1 = glm::dot(ray->direction, ray->direction);
    float k2 = 2 * glm::dot(oc, ray->direction);
    float k3 = glm::dot(oc, oc) - this->radius * this->radius;
    float discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0) {
        return -1;
    }
    float t1 = (-k2 - sqrt(discriminant)) / (2 * k1);
    float t2 = (-k2 + sqrt(discriminant)) / (2 * k1);
    if (t1 > EPSILON)return t1;
    if (t2 > EPSILON)return t2;
    return -1;
}

glm::vec3 Sphere::randomPoint() {
    return Ray::random_in_unit_sphere()*this->radius;
}

glm::vec3 Sphere::calculateNormal(glm::vec3 &point, glm::vec3 &dir) {
    return glm::normalize(point - this->position);
}

Material Sphere::getMaterial() {
    return material;
}

glm::vec3 Sphere::calculateUVCoordinates(glm::vec3 collisionPoint, glm::vec3 normal) {
    return glm::vec3(atan2(normal.x, normal.z) / (2 * M_PI) + 0.5, normal.y * 0.5 + 0.5, 0.0);
}
