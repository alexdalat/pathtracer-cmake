#pragma once

#include <chrono>
#include <glm/glm.hpp>

#include "Ray.h"
#include "Scene.h"

class Renderer {
 public:
  Scene* scene = nullptr;

  int width = 512, height = 512, samples = 250, recursion_depth = 8;
  float light_loss = 0.2f, min_dist = 0.000001f, max_dist = 100.f;

  std::string img_dir = "../imgs/";
  std::string img_name = "image";

  std::vector<glm::vec2> random_map;
  std::vector<Ray> ray_map;
  std::vector<glm::vec3> random_ray_map;

  Renderer();
  Renderer(Scene& scene);

  void init();
  glm::vec3 trace(Ray const& ray, int hash, int depth = 0);
  std::vector<glm::vec3> render();
};
