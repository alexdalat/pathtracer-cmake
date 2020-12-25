#include "Rotation.h"
#include <math.h>
#include <glm/glm.hpp>

Rotation::Rotation() {}

Rotation::Rotation(float pitch, float yaw, float roll) {
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;

    updatePitchMat();
    updateYawMat();
    updateRollMat();
}

void Rotation::updatePitchMat() {
    pitchMat = glm::mat3(1, 0, 0,
                         0, cos(pitch), -sin(pitch),
                         0, sin(pitch), cos(pitch));
}

void Rotation::updateYawMat() {
    yawMat = glm::mat3(cos(yaw), 0, sin(yaw),
                       0, 1, 0,
                       -sin(yaw), 0, cos(yaw));
}

void Rotation::updateRollMat() {
    rollMat = glm::mat3(cos(roll), -sin(roll), 0,
                        sin(roll), cos(roll), 0,
                        0, 0, 1);
}

