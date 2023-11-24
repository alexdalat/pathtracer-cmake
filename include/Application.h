#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Renderer.h"

static void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}


class Application {
 public:
  Application(Renderer& renderer);
  ~Application();

  Renderer& renderer;

  int init();
  void updateTexture(const std::vector<glm::vec3>& pixels);
  void run();

 private:
  GLuint textureID;
  GLuint vao, vbo;

  GLFWwindow* window = nullptr;

  void setupTexture();
  void setupQuad();
  void setupFramebuffer();
  void renderTexture();

  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  GLuint compileShader(GLenum type, const std::string& source);
  GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
};
