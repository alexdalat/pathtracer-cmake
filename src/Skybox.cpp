
#include "Skybox.h"

Skybox::Skybox(glm::vec3 ntopColor, glm::vec3 nsideColor,
               glm::vec3 nbottomColor, bool noverride, glm::vec3 noverrideColor,
               float nintensity, glm::vec3 nup)
    : topColor(ntopColor),
      sideColor(nsideColor),
      bottomColor(nbottomColor),
      override(noverride),
      overrideColor(noverrideColor),
      intensity(nintensity),
      up(nup) {}

glm::vec3 Skybox::getColorAt(glm::vec3 const& dir) {
  if (this->override) return this->overrideColor;

  float dot = glm::dot(this->up, dir);
  if (dot < 0.0f)
    return glm::mix(this->sideColor, this->bottomColor, std::abs(dot)) *
           this->intensity;

  return glm::mix(this->sideColor, this->topColor, dot) * this->intensity;
}
