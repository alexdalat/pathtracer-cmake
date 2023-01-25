#include "Animation.h"

#define M_PI 3.14159f

void Animation::update(Scene *scene, int i) {
    float angle = (2.f*M_PI) * ((float)i / (float)this->frames);
    glm::vec3 center(0, 3, 0);
    glm::vec3 start(0, 3, 15);
    scene->camera->position.x = cos(angle) * (start.x - center.x) - sin(angle) * (start.z - center.z) + center.x;
    scene->camera->position.z = sin(angle) * (start.x - center.x) + cos(angle) * (start.z - center.z) + center.z;
    scene->camera->lookAt(center);
}