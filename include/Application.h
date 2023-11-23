#pragma once

#include <OpenGL/OpenGL.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
 
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <vector>

#include "Renderer.h"
 
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
 
class Application {
public:
    Application(int w, int h);
    ~Application();

    void init();
    void setupTexture();
    void setupQuad();
    void updateTexture(const std::vector<glm::vec3>& pixels);
    void renderTexture();
    void setupFramebuffer();

    void run(Renderer& renderer);

private:
    GLuint textureID;
    GLuint fboId;
    int width, height;

    float* quadVertices;
    GLFWwindow* window;
};
