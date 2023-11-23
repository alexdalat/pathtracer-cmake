#pragma once

#include <OpenGL/OpenGL.h>
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

class Application {
 public:
  Application(int w, int h);
  ~Application();

  void updateTexture(const std::vector<glm::vec3>& pixels);
  void run(Renderer& renderer);

 private:
  GLuint textureID;
  GLuint vao, vbo;
  int width, height;

  GLFWwindow* window;

  void init();
  void setupTexture();
  void setupQuad();
  void setupFramebuffer();
  void renderTexture();

  GLuint compileShader(GLenum type, const std::string& source);
  GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
};
