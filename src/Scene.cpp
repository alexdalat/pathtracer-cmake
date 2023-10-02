
#include "Scene.h"

Scene::Scene() {}

void Scene::setSkybox(Skybox *skybox) {
    this->skybox = skybox;
}

void Scene::setCamera(Camera *camera) {
    this->camera = camera;
}

void Scene::addObject(std::unique_ptr<Object> object) {
    this->objects.emplace_back(move(object));
}

void Scene::addObjects(vector<std::unique_ptr<Object>> &vector) {
    for (int i = 0; i < vector.size(); i++)
        this->objects.emplace_back(move(vector[i]));
}

Intersection Scene::castRay(Ray *ray, Renderer *renderer, int ignoreIndex) {
    Object *object = nullptr;
    float closestDistance = -1;

    for (int i = 0; i < this->objects.size(); i++) {
        if(i == ignoreIndex)continue;
        float dist = this->objects[i].get()->calculateIntersection(ray);
        if (dist >= renderer->min_dist && dist < renderer->max_dist && (dist < closestDistance || closestDistance < 0)) {
            closestDistance = dist;
            object = this->objects[i].get();
        }
    }

    return Intersection({closestDistance > -1, closestDistance, ray->across(closestDistance), object});
}

void Scene::loadObjectFile(std::string filename, Material material, glm::vec3 translate, glm::vec3 scale) {
    std::ifstream f(filename);
    int vertexCount = 0;
    int faceCount = 0;
    std::vector<glm::vec3> vertices;
    std::string line;
    while (getline(f, line)) {
        line = std::regex_replace(std::regex_replace(line, std::regex(" +"), " "), std::regex("^ *| *$"), "");
        if (line.rfind("#", 0) == 0) {
            continue;
        }
        if (line.rfind("g ", 0) == 0) {
            //vertices.empty();
        }
        if (line.rfind("v ", 0) == 0) {
            float x, y, z;
            sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
            vertices.emplace_back(x, y, z);
            vertexCount++;
        }
        if (line.rfind("f ", 0) == 0) {
            if (std::regex_match(line, std::regex(R"(f( [0-9]+){3})", std::regex_constants::extended))) {
                int i0, i1, i2;
                sscanf(line.c_str(), "f %d %d %d", &i0, &i1, &i2);
                if (i0 > int(vertices.size())) {
                    std::cerr << i0 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                if (i1 > int(vertices.size())) {
                    std::cerr << i1 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                if (i2 > int(vertices.size())) {
                    std::cerr << i2 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                this->addObject(std::make_unique<Triangle>(Triangle(vertices[i0 - 1] * scale + translate, vertices[i1 - 1] * scale + translate, vertices[i2 - 1] * scale + translate, material)));
                faceCount++;
            } else if (std::regex_match(line, std::regex(R"(f( [0-9]+\/[0-9]+\/[0-9]+){3})", std::regex_constants::extended))) {
                int v0, v1, v2, vn0, vn1, vn2, vt0, vt1, vt2;
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &vn0, &vt0, &v1, &vn1, &vt1, &v2, &vn2, &vt2);
                if (v0 > int(vertices.size())) {
                    std::cerr << v0 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                if (v1 > int(vertices.size())) {
                    std::cerr << v1 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                if (v2 > int(vertices.size())) {
                    std::cerr << v2 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                this->addObject(std::make_unique<Triangle>(Triangle(vertices[v0 - 1] * scale + translate, vertices[v1 - 1] * scale + translate, vertices[v2 - 1] * scale + translate, material)));
                faceCount++;
            } else if (std::regex_match(line, std::regex(R"(f( [0-9]+\/\/[0-9]+){3})", std::regex_constants::extended))) {
                int v0, v1, v2, vt0, vt1, vt2;
                sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &vt0, &v1, &vt1, &v2, &vt2);

                if (v0 > int(vertices.size())) {
                    std::cerr << v0 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                if (v1 > int(vertices.size())) {
                    std::cerr << v1 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                if (v2 > int(vertices.size())) {
                    std::cerr << v2 << " is out of range for .obj vertex index" << std::endl;
                    std::cout << line << std::endl;
                    continue;
                }
                this->addObject(std::make_unique<Triangle>(Triangle(vertices[v0 - 1] * scale + translate, vertices[v1 - 1] * scale + translate, vertices[v2 - 1] * scale + translate, material)));
                faceCount++;
            } else {
                std::cerr << "Face does not match known format: " << line << std::endl;
            }
        }
    }
    std::cout << "Successfully imported OBJ with " << faceCount << " faces and " << vertexCount << " vertices" << std::endl;
}
