#pragma once
#ifndef RENDERER_H
#define RENDERER_H
using namespace std;

#include "Color.h"
#include "Scene.h"
#include "Object.h"
#include "Ray.h"
#include <vector>
#include <optional>

class Scene;

struct Intersection;

class Renderer {
public:
    Scene *scene;
    int width, height, samples, thread_count, recursion_depth;
    float light_loss, min_dist, max_dist;
    float viewport_size, projection_plane_z = 1;

    Renderer(Scene *scene, int width, int height, int samples, int thread_count = 1, int recursion_depth = 2,
             float light_loss = 0.2, float min_dist = 0.001, float max_dist = 10);

    Color renderPixel(Ray ray, int depth = 0);

    Intersection ClosestIntersection(Ray *ray);

    glm::vec3 CanvasToViewport(glm::vec2 p2d);

    void SetScene(Scene &scene);
};

#endif