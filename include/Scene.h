#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <regex>

#include "Camera.h"
#include "Object.h"
#include "Plane.h"
#include "Ray.h"
#include "Skybox.h"

struct Intersection {
  Object* object;
  float distance;
};

class Scene {
 public:
  Skybox* skybox;
  Camera* camera;
  std::vector<Object*> objects;

  Scene();
  ~Scene();

  void addObject(Object* const& object);
  void addObjects(std::vector<Object*> const& vector);
  void loadObjectFile(std::string filename, Material const& material,
                      glm::vec3 const& translate, glm::vec3 const& scale);
  void setupScene(std::string const& obj_path);
  void destroyScene();

  Intersection castRay(Ray const& ray, float const min_dist,
                       float const max_dist);
};
