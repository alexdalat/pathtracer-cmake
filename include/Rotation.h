#pragma once

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
