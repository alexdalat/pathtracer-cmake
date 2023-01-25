#ifndef PATHTRACER_CMAKE_DATA_H
#define PATHTRACER_CMAKE_DATA_H

struct Intersection {
    bool hit;
    double distance;
    glm::vec3 point; // collision point
    Object *object;
};

#endif //PATHTRACER_CMAKE_DATA_H
