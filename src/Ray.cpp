using namespace std;

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/norm.hpp>
#include <random>

#include "Ray.h"
#include "Util.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}

glm::vec3 Ray::across(float distance) {
    return origin + (direction * distance);
}

glm::vec3 Ray::random_in_unit_sphere() {
    glm::vec3 randomDirection = glm::normalize(glm::vec3({
         randomDouble() * 2 - 1,
         randomDouble() * 2 - 1,
         randomDouble() * 2 - 1,
     }));
    return randomDirection;
}

glm::vec3 Ray::diffuse(glm::vec3 normal) {
    return normal + Ray::random_in_unit_sphere();
}

glm::vec3 Ray::reflect(Ray ray, glm::vec3 normal) {
    return ray.direction - (2 * glm::dot(ray.direction, glm::normalize(normal)) * glm::normalize(normal));
}