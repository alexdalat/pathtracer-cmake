#ifndef PATHTRACER_CMAKE_ANIMATION_H
#define PATHTRACER_CMAKE_ANIMATION_H

#include "Scene.h"
#include <iostream>

class Scene;

class Animation {
public:
    Animation(){};
    const int frames = 360;

    void update(Scene *scene, int i);

};
#endif //PATHTRACER_CMAKE_ANIMATION_H
