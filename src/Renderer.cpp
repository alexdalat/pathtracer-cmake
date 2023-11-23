#include "Renderer.h"

void Renderer::init() {

  for (int i = 0; i < this->width * this->height * samples; i++) {
    random_map.push_back(glm::vec2(util::random(), util::random()));
  }

  for (int x = 0; x < this->width; x++) {
    for (int y = 0; y < this->height; y++) {
      for (int s = 0; s < samples; s++) {
        float u, v;
        if (this->samples > 1) {  // 0.0 - 1.0 for centering
          glm::vec2& rand = random_map.at((y * s + x * s * this->height) %
                                       (this->width * this->height));

          u = ((float)x + rand.x) / (float)this->width;
          v = ((float)y + rand.y) / (float)this->height;
        } else {                                       // sampling = 1
          u = ((float)x + 0.5f) / (float)this->width;  // 0.5 for centering
          v = ((float)y + 0.5f) / (float)this->height;
        }
        ray_map.push_back(this->scene->camera->getRay(u, v));
      }
    }
  }
}

Renderer::Renderer() {}

Renderer::Renderer(Scene& scene) {
  this->scene = &scene;
}

glm::vec3 Renderer::trace(Ray const& ray, int depth) {
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
  if (material.diffuse > 0) ndir = Ray::diffuse(normal);

  if (material.reflectivity > 0) ndir = Ray::reflect(ray, normal);

  // if (material.diffuse > 0 && material.reflectivity > 0 && util::random() <
  // 0.5)
  //   ndir = diffuseDir;

  if (material.emissive <= 0)
    return this->trace(Ray(point, ndir), depth + 1) * material.color *
           (1.f - this->light_loss);
  else
    return material.color * material.emissive;
}

void Renderer::render(std::vector<glm::vec3>& pixels) {
  // start time
  auto start = std::chrono::high_resolution_clock::now();

  if(this->ray_map.size() == 0 || this->random_map.size() == 0) this->init();

  for (int x = 0; x < this->width; x++) {
    for (int y = 0; y < this->height; y++) {
      glm::vec3 cols(0.0f);

      for (int s = 0; s < this->samples; s++) {
        Ray ray = ray_map.at(y * samples + x * this->height * samples  + s);
        // float u = ((float)x + 0.5f) / (float)this->width;
        // float v = ((float)y + 0.5f) / (float)this->height;
        // Ray ray = this->scene->camera->getRay(u, v);
        glm::vec3 col = this->trace(ray);
        cols += col;
      }

      cols /= static_cast<float>(this->samples);
      cols = glm::clamp(cols, 0.0f, 1.0f);

      int pc = (this->height - y - 1) * this->width + x; // flip y for opengl (draws bottom left)
      pixels[pc] = cols;
      // pixels[pc] = glm::vec3((float)x / (float)this->width, (float)y / (float)this->height, 0.2f); // testing
    }
  }
  std::cout << "Rendered " << this->width << "x" << this->height << " image in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << "ms\n";
}
