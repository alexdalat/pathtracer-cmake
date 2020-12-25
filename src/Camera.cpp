using namespace std;

#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, float aspectRatio, float fov, Rotation rotation) {
    this->position = position;
    this->rotation = rotation;
    this->fov = fov;
    this->aspectRatio = aspectRatio;
}

Ray Camera::getRay(double x, double y) {
    glm::vec3 direction = glm::normalize(glm::vec3((x - 0.5) * (this->fov / 180), (y - 0.5) * (this->fov / 180) * this->aspectRatio, 1));

    direction = direction * rotation.rollMat;
    direction = direction * rotation.pitchMat;
    direction = direction * rotation.yawMat;
    direction = glm::normalize(direction);

    return Ray(position, direction);
}