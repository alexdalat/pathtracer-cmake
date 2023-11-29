#pragma once

#include <glm/glm.hpp>

#include "Ray.h"
#include "Scene.h"

class Renderer {
 public:
  Scene* scene = nullptr;

  int width = 400, height = 400, samples = 5, jitter = 4, recursion_depth = 6;
  float light_loss = 0.2f, min_dist = 0.0001f, max_dist = 500.f;

  std::string img_dir = "../imgs/";
  std::string img_name = "image";

  size_t random_map_size = 2713543; // prime
  std::vector<glm::vec2> random_map;
  std::vector<glm::vec3> random_ray_map;

  std::vector<std::vector<std::vector<glm::vec3>>> ray_map;

  Renderer();
  Renderer(Scene& scene);

  void generateRandoms();

  void addRay(size_t x, size_t y, size_t s);
  void generateRays();
  glm::vec3 trace(Ray& ray, int hash, int depth = 0);
  void render(std::vector<glm::vec3>* const pixels);
};
