#include "Scene.h"

#include "Plane.h"
#include "Shapes.h"
#include "Triangle.h"

Scene::Scene() {}
Scene::~Scene() { this->destroyScene(); }

void Scene::addObject(Object* const& object) {
  this->objects.emplace_back(std::move(object));
}

void Scene::addObjects(std::vector<Object*> const& vector) {
  for (int i = 0; i < vector.size(); i++)
    this->objects.emplace_back(std::move(vector[i]));
}

Intersection Scene::castRay(Ray const& ray, float const min_dist,
                            float const max_dist) {
  Object* object = nullptr;
  float closestDistance = max_dist;

  for (int i = 0; i < this->objects.size(); i++) {
    float dist = this->objects[i]->calculateIntersection(ray);

    if (dist >= min_dist && dist < max_dist && dist < closestDistance) {
      closestDistance = dist;
      object = this->objects[i];
    }
  }
  return Intersection{object, closestDistance};
}

void Scene::setupScene(std::string const& obj_path) {
  this->addObject(
      new Plane(glm::vec3(0.0, 0, 0.0), glm::vec3(0.0, 1.0, 0.0),
                Material(0.0, 1.0, 0.0, glm::vec3(255.0, 255.0, 255.0)/255.0f)));

  /** Floor **/
  // auto floor = Rect(glm::vec3(5, 0, -5), glm::vec3(-5, 0, -5), glm::vec3(-5,
  // 0, 5), glm::vec3(5, 0, 5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
  // scene->addObjects(floor); // floor tile

  // Lights //
  this->addObjects(Rect(glm::vec3(-3, 9.95, -3), glm::vec3(-3, 9.95, -1),
                        glm::vec3(-1, 9.95, -1), glm::vec3(-1, 9.95, -3),
                        Material(0.0, 0.0, 40.0, glm::vec3(253, 173, 92)/255.0f)));

  /*
  // Walls //
  auto wall1 = Rect(glm::vec3(-5, 0, -5), glm::vec3(-5, 10, -5), glm::vec3(-5,
  10, 5), glm::vec3(-5, 0, 5), Material(0.0, 1.0, 0.0, Color(175, 0, 0)));
  scene->addObjects(wall1); // left
  auto wall2 = Rect(glm::vec3(5, 0, -5), glm::vec3(5, 0, 5), glm::vec3(5, 10,
  5),glm::vec3(5, 10, -5), Material(0.0, 1.0, 0.0, Color(0, 175, 0)));
  scene->addObjects(wall2); // right

  auto wall3 = Rect(glm::vec3(-5, 0, -5), glm::vec3(5, 0, -5), glm::vec3(5, 10,
  -5), glm::vec3(-5, 10, -5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
  scene->addObjects(wall3); // straight
  //auto wall4 = Rect(glm::vec3(-5, 0, 10), glm::vec3(-5, 10, 10), glm::vec3(5,
  10, 10), glm::vec3(5, 0, 5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
  //scene->addObjects(wall4); // back

  // Roof //
  auto roof = Rect(glm::vec3(-5,10, -5), glm::vec3(5, 10, -5), glm::vec3(5, 10,
  5), glm::vec3(-5, 10, 5), Material(0.0, 1.0, 0.0, Color(175, 175, 175)));
  scene->addObjects(roof);

  // Box 1 (RECT PRISM) //
  const Material box1mat = Material(1.0, 0.0, 0.0, Color(255, 255, 255));
  auto box1wallleft = Rect(glm::vec3(-2.5, 0, 1), glm::vec3(-3.5, 0, -2),
  glm::vec3(-3.5, 6, -2),glm::vec3(-2.5, 6, 1), box1mat);
  scene->addObjects(box1wallleft); // left
  auto box1wallright = Rect(glm::vec3(0.5, 0, 0), glm::vec3(-0.5, 0, -3),
  glm::vec3(-0.5, 6, -3),glm::vec3(0.5, 6, 0), box1mat);
  scene->addObjects(box1wallright); // right
  auto box1wallfront = Rect(glm::vec3(-2.5, 0, 1), glm::vec3(0.5, 0, 0),
  glm::vec3(0.5, 6, 0), glm::vec3(-2.5, 6, 1), box1mat);
  scene->addObjects(box1wallfront); // front
  auto box1wallback = Rect(glm::vec3(-3.5, 0, -2), glm::vec3(-0.5, 0, -3),
  glm::vec3(-0.5, 6, -3), glm::vec3(-3.5, 6, -2), box1mat);
  scene->addObjects(box1wallback); // back
  auto box1wallroof = Rect(glm::vec3(-3.5, 6, -2), glm::vec3(-2.5, 6, 1),
  glm::vec3(0.5, 6, 0), glm::vec3(-0.5, 6, -3), box1mat);
  scene->addObjects(box1wallroof); // roof

  // Box 2 (CUBE) //
  const Material box2mat = Material(0.0, 1.0, 0.0, Color(255, 255, 255));
  auto box2wallleft = Rect(glm::vec3(0, 0, 3), glm::vec3(1, 0, 0), glm::vec3(1,
  3, 0),glm::vec3(0, 3, 3), box2mat); scene->addObjects(box2wallleft); // left
  auto box2wallright = Rect(glm::vec3(3, 0, 4), glm::vec3(4, 0, 1), glm::vec3(4,
  3, 1),glm::vec3(3, 3, 4), box2mat); scene->addObjects(box2wallright); // right
  auto box2wallfront = Rect(glm::vec3(0, 0, 3), glm::vec3(3, 0, 4), glm::vec3(3,
  3, 4), glm::vec3(0, 3, 3), box2mat); scene->addObjects(box2wallfront); //
  front auto box2wallback = Rect(glm::vec3(1, 0, 0), glm::vec3(4, 0, 1),
  glm::vec3(4, 3, 1), glm::vec3(1, 3, 0), box2mat);
  scene->addObjects(box2wallback); // back
  auto box2wallroof = Rect(glm::vec3(1, 3, 0), glm::vec3(0, 3, 3), glm::vec3(3,
  3, 4), glm::vec3(4, 3, 1), box2mat); scene->addObjects(box2wallroof); // roof
  */

  // Objects //
  // this->loadObjectFile(obj_path + "teddy.obj", Material(0.0f, 1.0f, 0.0f,
  // glm::vec3(255, 0, 0)), glm::vec3(0, 3, 0), glm::vec3(0.15));
}

void Scene::destroyScene() {
  for (int i = 0; i < this->objects.size(); i++) {
    delete this->objects[i];
  }
  this->objects.clear();
}

void Scene::loadObjectFile(std::string filename, Material const& material,
                           glm::vec3 const& translate, glm::vec3 const& scale) {
  std::ifstream f(filename);
  int vertexCount = 0;
  int faceCount = 0;
  std::vector<glm::vec3> vertices;
  std::string line;
  while (getline(f, line)) {
    line = std::regex_replace(std::regex_replace(line, std::regex(" +"), " "),
                              std::regex("^ *| *$"), "");
    if (line.rfind("#", 0) == 0) {
      continue;
    }
    if (line.rfind("g ", 0) == 0) {
      // vertices.empty();
    }
    if (line.rfind("v ", 0) == 0) {
      float x, y, z;
      sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
      vertices.emplace_back(x, y, z);
      vertexCount++;
    }
    if (line.rfind("f ", 0) == 0) {
      if (std::regex_match(line, std::regex(R"(f( [0-9]+){3})",
                                            std::regex_constants::extended))) {
        int i0, i1, i2;
        sscanf(line.c_str(), "f %d %d %d", &i0, &i1, &i2);
        if (i0 > int(vertices.size())) {
          std::cerr << i0 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        if (i1 > int(vertices.size())) {
          std::cerr << i1 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        if (i2 > int(vertices.size())) {
          std::cerr << i2 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        this->addObject(new Triangle(vertices[i0 - 1] * scale + translate,
                                     vertices[i1 - 1] * scale + translate,
                                     vertices[i2 - 1] * scale + translate,
                                     material));
        faceCount++;
      } else if (std::regex_match(line,
                                  std::regex(R"(f(
            [0-9]+\/[0-9]+\/[0-9]+){3})",
                                             std::regex_constants::extended))) {
        int v0, v1, v2, vn0, vn1, vn2, vt0, vt1, vt2;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &vn0, &vt0,
               &v1, &vn1, &vt1, &v2, &vn2, &vt2);
        if (v0 > int(vertices.size())) {
          std::cerr << v0 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        if (v1 > int(vertices.size())) {
          std::cerr << v1 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        if (v2 > int(vertices.size())) {
          std::cerr << v2 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        this->addObject(new Triangle(vertices[v0 - 1] * scale + translate,
                                     vertices[v1 - 1] * scale + translate,
                                     vertices[v2 - 1] * scale + translate,
                                     material));
        faceCount++;
      } else if (std::regex_match(line,
                                  std::regex(R"(f(
            [0-9]+\/\/[0-9]+){3})",
                                             std::regex_constants::extended))) {
        int v0, v1, v2, vt0, vt1, vt2;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &vt0, &v1, &vt1,
               &v2, &vt2);

        if (v0 > int(vertices.size())) {
          std::cerr << v0 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        if (v1 > int(vertices.size())) {
          std::cerr << v1 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        if (v2 > int(vertices.size())) {
          std::cerr << v2 << " is out of range for .obj vertex index"
                    << std::endl;
          std::cout << line << std::endl;
          continue;
        }
        this->addObject(new Triangle(vertices[v0 - 1] * scale + translate,
                                     vertices[v1 - 1] * scale + translate,
                                     vertices[v2 - 1] * scale + translate,
                                     material));
        faceCount++;
      } else {
        std::cerr << "Face does not match known format: " << line << std::endl;
      }
    }
  }
  std::cout << "Successfully imported OBJ with " << faceCount << " faces and "
            << vertexCount << " vertices" << std::endl;
}
