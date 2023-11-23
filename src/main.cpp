
#include <cmath>
#include <future>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

// project specific
#include "Application.h"
#include "Camera.h"
#include "Material.h"
#include "Plane.h"
#include "Renderer.h"
#include "Rotation.h"
#include "Scene.h"
#include "Shapes.h"
#include "Skybox.h"
#include "util.h"
//
// // input paths
const std::string obj_path = "../../extra/objs/";
// // output paths
const std::string img_dir = "./";
const std::string img_name = util::getFileTimestamp();

template <typename T>
T default_input(std::string var_name, T variable) {
  std::cout << var_name + std::string(" [default = ") +
                   std::to_string(variable) + "]: ";
  std::string input;
  std::getline(std::cin, input);
  if (!input.empty()) {
    std::istringstream strm(input);
    strm >> variable;
  }
  return variable;
}

int main() {
  Renderer renderer;
  std::cout << "\n";
  renderer.samples = default_input<int>("Samples", 5);
  renderer.width = default_input<int>("Width", 512);
  renderer.height = default_input<int>("Height", renderer.width);
  renderer.recursion_depth = default_input<int>("Recursion", 2);
  renderer.light_loss = default_input<float>("Light loss", 0.2f);
  renderer.img_dir = img_dir;
  renderer.img_name = img_name;

  Scene scene;
  renderer.scene = &scene;

  Camera camera(glm::vec3(0, 3, 15),  // normal: 0, 5, 15 | top-down: 0, 15, 0
                (float)renderer.height / (float)renderer.width, 180,
                Rotation(0, 0, 0)  // normal: 0,0,0 | top-down: -M_PI/2,0,0
  );
  camera.lookAt(glm::vec3(0, 0, 0));
  scene.camera = &camera;

  Skybox skybox(
      glm::vec3(63, 178, 232) / 255.0f, glm::vec3(225, 244, 252) / 255.0f,
      glm::vec3(225, 244, 252) / 255.0f, false, glm::vec3(0, 0, 0) / 255.0f);
  // red to blue to blue to green gradient skybox
  // Skybox skybox(glm::vec3(255, 0, 0) / 255.0f, glm::vec3(0, 255, 0) / 255.0f,
  //              glm::vec3(0, 0, 255) / 255.0f);
  scene.skybox = &skybox;

  scene.setupScene(obj_path);

  Application app(renderer.width, renderer.height);
  int status = app.init();
  if(status != 0) {
    std::cout << "Failed to initialize application, rendering without OpenGL." << std::endl;
    std::vector<glm::vec3> pixels(renderer.width * renderer.height, glm::vec3(0.0f));
    while(true) {
      renderer.render(&pixels);
    }
  } else {
    app.run(renderer);
  }

  return 0;
}
