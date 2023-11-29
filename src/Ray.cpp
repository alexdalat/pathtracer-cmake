
#include "Ray.h"

// glm::vec3 Ray::random_in_unit_sphere() {
//   return glm::normalize(glm::vec3({
//       util::random() * 2.0f - 1.0f,
//       util::random() * 2.0f - 1.0f,
//       util::random() * 2.0f - 1.0f,
//   }));  // x = [-1, 1], y = [-1, 1], z = [-1, 1]
// }

glm::vec3 Ray::diffuse(glm::vec3 const& normal, glm::vec3 const& rand_vec) {
  return glm::normalize(normal + rand_vec);
}

glm::vec3 Ray::reflect(Ray const& ray, glm::vec3 const& normal) {
  return ray.direction - (2.0f * glm::dot(ray.direction, normal) * normal);
}
