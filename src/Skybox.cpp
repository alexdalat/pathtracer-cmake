
#include "Skybox.h"

#include <iostream>

Skybox::Skybox(glm::vec3 ntopColor, glm::vec3 nsideColor,
               glm::vec3 nbottomColor, bool noverride, glm::vec3 noverrideColor,
               float nintensity, glm::vec3 nup)
    : topColor(ntopColor),
      sideColor(nsideColor),
      bottomColor(nbottomColor),
      override(noverride),
      overrideColor(noverrideColor),
      intensity(nintensity),
      up(nup) {
  this->bsColor = this->bottomColor - this->sideColor;
  this->tsColor = this->topColor - this->sideColor;
}

glm::vec3 Skybox::getColorAt(glm::vec3 const& dir) {
  if (this->override) return this->overrideColor;

  float dot = glm::dot(this->up, dir);
  if (dot < 0.0f)
    return this->sideColor + this->bsColor * std::abs(dot);
  return this->sideColor + this->tsColor * dot;
}
