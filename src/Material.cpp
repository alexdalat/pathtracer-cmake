
#include "Material.h"

Material::Material() {
  this->reflectivity = 0;
  this->diffuse = 0;
  this->emissive = 0;
  this->color = glm::vec3(0.0f);
}

Material::Material(float reflectivity, float diffuse, float emissive,
                   glm::vec3 color) {
  this->reflectivity = reflectivity;
  this->diffuse = diffuse;
  this->emissive = emissive;
  this->color = color;
}
