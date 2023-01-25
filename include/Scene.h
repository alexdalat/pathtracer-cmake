#pragma once
#ifndef SCENE_H
#define SCENE_H
using namespace std;

#include <vector>
#include "Renderer.h"
#include "Camera.h"
#include "Skybox.h"
#include "Ray.h"
#include "Object.h"
#include "Util.h"
#include "Data.h"

class Renderer;

class Scene {
public:
    Skybox *skybox;
    Camera *camera;
    std::vector<std::unique_ptr<Object>> objects;

    Scene();
    void setSkybox(Skybox *skybox);
    void setCamera(Camera *camera);
    void addObject(unique_ptr<Object> object);
    void addObjects(vector<unique_ptr<Object>> &vector);
    Intersection castRay(Ray *ray, Renderer *renderer, int ignoreIndex = -1);
    Intersection testRandomEmissive(glm::vec3 point, Renderer *renderer);
    void loadObjectFile(std::string filename, Material material, glm::vec3 translate, glm::vec3 scale);

};

#endif