#pragma once

#include <chrono>
#include <glm/glm.hpp>

#include "Ray.h"
#include "Scene.h"

class Renderer {
 public:
  Scene* scene = nullptr;

  int width = 128, height = 128, samples = 5, recursion_depth = 8;
  float light_loss = 0.2f, min_dist = 0.0001f, max_dist = 500.f;

  std::string img_dir = "../imgs/";
  std::string img_name = "image";

  std::vector<glm::vec2> random_map;
  std::vector<Ray> ray_map;
  std::vector<glm::vec3> random_ray_map;

  Renderer();
  Renderer(Scene& scene);

  void init();
  void generateRays();
  glm::vec3 trace(Ray const& ray, int hash, int depth = 0);
  void render(std::vector<glm::vec3>* const pixels);
};
