
#include "Material.h"

Material::Material() {
    this->reflectivity = 0;
    this->diffuse = 0;
    this->emissive = 0;
    this->color = Color();
}

Material::Material(float reflectivity, float diffuse, float emissive, Color color) {
    this->reflectivity = reflectivity;
    this->diffuse = diffuse;
    this->emissive = emissive;
    this->color = color;
}
