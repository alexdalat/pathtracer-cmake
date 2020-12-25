#ifndef PATHTRACER_ROTATION_H
#define PATHTRACER_ROTATION_H

#include <glm/glm.hpp>

class Rotation {
public:
    float pitch;
    float yaw;
    float roll;
    glm::mat3 yawMat;
    glm::mat3 pitchMat;
    glm::mat3 rollMat;

    Rotation();

    Rotation(float pitch, float yaw, float roll);

    void updatePitchMat();

    void updateYawMat();

    void updateRollMat();
};

#endif //PATHTRACER_ROTATION_H
