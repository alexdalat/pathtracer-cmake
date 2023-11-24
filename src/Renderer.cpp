#include "Renderer.h"

void Renderer::init() {
  for (int i = 0; i < this->width * this->height * samples; i++) {
    if (i < this->random_map.size()) continue;

    glm::vec2 rand = glm::vec2(util::random(), util::random());
    random_map.push_back(rand);
    random_ray_map.push_back(glm::normalize(glm::vec3(rand.x, rand.y, util::random())));
  }

  this->generateRays();
}

void Renderer::generateRays() {
  ray_map.clear();
  for (int x = 0; x < this->width; x++) {
    for (int y = 0; y < this->height; y++) {
      for (int s = 0; s < this->samples; s++) {
        int hash = y * samples + x * this->height * samples + s;
        glm::vec2& rand = random_map[hash % (this->width * this->height)];
        float u = ((float)x + rand.x) / (float)this->width;
        float v = ((float)y + rand.y) / (float)this->height;
        ray_map.push_back(this->scene->camera->getRay(u, v));
      }
    }
  }
  this->scene->camera->needUpdate = false;
}

Renderer::Renderer() {}

Renderer::Renderer(Scene& scene) { this->scene = &scene; }

glm::vec3 Renderer::trace(Ray const& ray, int hash, int depth) {
  // max depth reached = dark
  if (depth >= this->recursion_depth) return glm::vec3(0.0f);

  Intersection intersection =
      this->scene->castRay(ray, this->min_dist, this->max_dist);
  if (intersection.object == nullptr) {
    if (scene->skybox) return scene->skybox->getColorAt(ray.direction);
    return this->scene->skybox->defaultColor;
  }

  Object* object = intersection.object;
  glm::vec3 point = ray.origin + (ray.direction * intersection.distance);

  glm::vec3 normal = object->calculateNormal(point, ray.direction);
  Material& material = object->material;

  glm::vec3 ndir;
  if (material.diffuse > 0) ndir = Ray::diffuse(normal, random_ray_map[hash + depth % this->width*this->height*this->samples]);

  if (material.reflectivity > 0) ndir = Ray::reflect(ray, normal);

  // if (material.diffuse > 0 && material.reflectivity > 0 && util::random() <
  // 0.5)
  //   ndir = diffuseDir;

  if (material.emissive <= 0)
    return this->trace(Ray(point, ndir), hash, ++depth) * material.color *
           (1.0f - this->light_loss);
  else
    return material.color * material.emissive;
}

void Renderer::render(std::vector<glm::vec3>* const pixels) {

  if (this->ray_map.size() == 0 || this->random_map.size() == 0) this->init();
  
  // this->scene->camera->needUpdate = true; for profiling
  if(this->scene->camera->needUpdate) this->generateRays();

  for (int x = 0; x < this->width; x++) {
    for (int y = 0; y < this->height; y++) {
      glm::vec3 cols(0.0f);

      int h = y * samples + x * this->height * samples;
      for (int s = 0; s < this->samples; s++) {
        int hash = h + s;
        cols += this->trace(ray_map.at(hash), hash);
      }

      cols /= static_cast<float>(this->samples);
      // cols = glm::clamp(cols, 0.0f, 1.0f);

      int pc = (this->height - y - 1) * this->width +
               x;  // flip y for opengl (draws bottom left)
      pixels->at(pc) = cols;
      // pixels[pc] = glm::vec3((float)x / (float)this->width, (float)y /
      // (float)this->height, 0.2f); // testing
    }
  }

}
