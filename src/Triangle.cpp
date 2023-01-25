using namespace std;

#include "Triangle.h"

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Material material) {
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->material = material;
    //this->texture = texture;
}

float Triangle::calculateIntersection(Ray *ray) { // Möller–Trumbore intersection algorithm
    glm::vec3 edge1 = this->v2 - this->v1;
    glm::vec3 edge2 = this->v3 - this->v1;
    glm::vec3 pvec = glm::cross(ray->direction, edge2);
    float det = glm::dot(edge1, pvec);
    if (det > -0.000001 && det < 0.000001) return -1;
    float invDet = 1.0f / det;
    glm::vec3 tvec = ray->origin - this->v1;
    float u = glm::dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return -1;
    glm::vec3 qvec = glm::cross(tvec, edge1);
    float v = glm::dot(ray->direction, qvec) * invDet;
    if (v < 0 || u + v > 1) return -1;
    return glm::dot(edge2, qvec) * invDet; // return distance
}

glm::vec3 Triangle::calculateNormal(glm::vec3 &point, glm::vec3 &dir) {
    glm::vec3 edge1 = this->v2 - this->v1;
    glm::vec3 edge2 = this->v3 - this->v1;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
    if(dot(dir,normal) < 0) return normal;
    else return -normal;
}

Material Triangle::getMaterial() {
    return this->material;
}