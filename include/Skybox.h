#pragma once

#include <glm/glm.hpp>

class Skybox {
 public:
  glm::vec3 up;
  glm::vec3 topColor;
  glm::vec3 sideColor;
  glm::vec3 bottomColor;
  bool override;
  glm::vec3 overrideColor;
  float intensity;

  const glm::vec3 defaultColor =
      glm::vec3(255.0f, 105.0f, 180.0f) / 255.0f;  // hot pink

  glm::vec3 bsColor;
  glm::vec3 tsColor;

  Skybox(glm::vec3 topColor = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 sideColor = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 bottomColor = glm::vec3(0.0f, 0.0f, 0.0f),
         bool override = false,
         glm::vec3 overrideColor = glm::vec3(1.0f, 0.1f, 0.5f),
         float intensity = 1, glm::vec3 up = glm::vec3(0, 1, 0));

  glm::vec3 getColorAt(glm::vec3 const& dir);
};
