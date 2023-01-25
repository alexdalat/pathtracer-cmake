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
#include "Animation.h"

class Scene;
class Animation;

struct Intersection;

class Renderer {
public:
    Scene *scene = nullptr;
    int width = 512, height = 512, samples = 250, thread_count = 16, recursion_depth = 8;
    float light_loss = 0.2f, min_dist = 0.000001f, max_dist = 100.f;
    int atrous = 0;
    std::string img_dir = "../imgs/";
    std::string img_name = "image";
    std::vector<glm::vec3> C, N, P;
    Animation *animation = nullptr;

    Renderer() {};
    Color trace(Ray ray, int depth = 0);

    void setSamples(int val) { this->samples = std::move(val); }
    void setWidth(int val) { this->width = std::move(val); }
    void setHeight(int val) { this->height = std::move(val); }
    void setThreadCount(int val) { this->thread_count = std::move(val); }
    void setRecursionDepth(int val) { this->recursion_depth = std::move(val); }
    void setLightLoss(float val) { this->light_loss = std::move(val); }
    void setMinDist(float val) { this->min_dist = std::move(val); }
    void setMaxDist(float val) { this->max_dist = std::move(val); }
    void setImageDir(std::string val) { this->img_dir = std::move(val); }
    void setImageName(std::string val) { this->img_name = std::move(val); }
    void setAtrous(bool val) { this->atrous = std::move(val); C.resize(width*height, glm::vec3(0)); N.resize(width*height, glm::vec3(0)); P.resize(width*height, glm::vec3(0)); }
    void setScene(Scene *scene) { this->scene = scene; }
    void setAnimation(Animation *animation) { this->animation = animation; }
};

#endif