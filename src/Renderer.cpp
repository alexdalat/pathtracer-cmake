#include "Renderer.h"

#include <iostream>

void Renderer::generateRandoms() {
  // reserve
  this->random_map.reserve(this->random_map_size);
  this->random_ray_map.reserve(this->random_map_size);

  for (int i = this->random_map.size(); i < this->random_map_size; i++) {
    glm::vec2 rand = glm::vec2(util::random(), util::random());
    random_map.push_back(rand);
    random_ray_map.push_back(
        glm::normalize(glm::vec3(rand.x, rand.y, util::random()) * 2.0f - 1.0f));
  }
}

void Renderer::addRay(size_t x, size_t y, size_t j) {
  size_t hash =
      (y * jitter + x * this->height * jitter + j) % this->random_map_size;
  glm::vec2& rand = random_map.at(hash);

  float u = ((float)x + rand.x) / (float)this->width;
  float v = ((float)y + rand.y) / (float)this->height;
  this->ray_map.at(x).at(y).at(j) = this->scene->camera->getRay(u, v);
}

void Renderer::generateRays() {
  if (this->scene == nullptr || this->scene->camera == nullptr) {
    std::cerr << "Scene or camera not initialized" << std::endl;
    exit(1);
  }
  if (this->random_map.size() == 0) {
    std::cerr << "Renderer not initialized, call Renderer::init()" << std::endl;
    exit(1);
  }

  for (size_t x = 0; x < this->width; x++) {
    this->ray_map.push_back(std::vector<std::vector<glm::vec3>>());
    for (size_t y = 0; y < this->height; y++) {
      this->ray_map.at(x).push_back(std::vector<glm::vec3>());
      for (size_t j = 0; j < this->jitter; j++) {
        this->ray_map.at(x).at(y).push_back(glm::vec3(0.0f));
        addRay(x, y, j);
      }
    }
  }
  this->scene->camera->needUpdate = false;
}

Renderer::Renderer() {}

Renderer::Renderer(Scene& scene) { this->scene = &scene; }

glm::vec3 Renderer::trace(Ray& ray, int hash, int depth) {
  // max depth reached = dark
  if (depth >= this->recursion_depth) return glm::vec3(0.0f);

  Intersection intersection =
      this->scene->castRay(ray, this->min_dist, this->max_dist);
  if (intersection.object == nullptr) {
    if (scene->skybox) return scene->skybox->getColorAt(ray.direction);
    return this->scene->skybox->defaultColor;
  }

  Object* object = intersection.object;
  ray.origin = ray.origin +
               (ray.direction * intersection.distance);  // point of collision

  glm::vec3 normal = object->calculateNormal(ray.origin, ray.direction);
  Material& material = object->material;

  if (material.diffuse > 0)
    ray.direction = Ray::diffuse(
        normal, random_ray_map.at((hash + depth) % this->random_map_size));

  if (material.reflectivity > 0) ray.direction = Ray::reflect(ray, normal);

  // if (material.diffuse > 0 && material.reflectivity > 0 && util::random() <
  // 0.5)
  //   ndir = diffuseDir;

  if (material.emissive <= 0)
    return this->trace(ray, hash, ++depth) * material.color *
           (1.0f - this->light_loss);
  else
    return material.color * material.emissive;
}

void Renderer::render(std::vector<glm::vec3>* const pixels) {
  if (this->scene == nullptr || this->scene->camera == nullptr) {
    std::cerr << "Scene or camera not initialized" << std::endl;
    exit(1);
  }

  if (this->ray_map.size() < this->width ||
      this->ray_map.at(0).size() < this->height ||
      this->ray_map.at(0).at(0).size() < this->jitter) {
    std::cerr << "Ray map not valid, call Renderer::generateRays()"
              << std::endl;
    std::cout << "ray_map size: " << this->ray_map.size() << " " << this->width << std::endl;
    std::cout << "ray_map[0] size: " << this->ray_map.at(0).size() << " " << this->height << std::endl;
    std::cout << "ray_map[0][0] size: " << this->ray_map.at(0).at(0).size() << " " << this->jitter << std::endl;

    exit(1);
  }

  if (this->random_map.size() != this->random_map_size) {
    std::cerr << "Random map not valid, call Renderer::generateRandoms()"
              << std::endl;
    exit(1);
  }

  // this->scene->camera->needUpdate = true; for profiling
  if (this->scene->camera->needUpdate) this->generateRays();

  for (int x = 0; x < this->width; x++) {
    for (int y = 0; y < this->height; y++) {
      glm::vec3 cols(0.0f);

      // int h = y + x * this->height;
      for (int s = 0; s < this->samples; s++) {
        for (int j = 0; j < this->jitter; j++) {
          int hash = (y * samples * jitter + x * this->height * samples * jitter + s + j) %
                     this->random_map_size;

          glm::vec3& dir = ray_map.at(x).at(y).at(j);
          Ray ray(this->scene->camera->position, dir);
          cols += this->trace(ray, hash);
        }
      }

      cols /= static_cast<float>(this->samples * this->jitter);
      // cols = glm::clamp(cols, 0.0f, 1.0f);

      int pc = y * this->width + x;  // flip y for opengl (draws bottom left)
      pixels->at(pc) = cols;
      // pixels[pc] = glm::vec3((float)x / (float)this->width, (float)y /
      // (float)this->height, 0.2f); // testing
    }
  }
}
