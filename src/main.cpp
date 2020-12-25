#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <future>

#include <png++/png.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// project specific
#include "RenderHelper.h"
#include "Color.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Ray.h"
#include "Util.h"
#include "Material.h"
#include "Camera.h"
#include "Skybox.h"
#include "Renderer.h"
#include "Filter.h"
#include "Scene.h"
#include "Rotation.h"
#include "Data.h"
#include "Shapes.h"

#define M_PI 3.14159f

const std::string img_output_path = "../imgs/";
const std::string obj_path = "../../extra/objs/";

void setupScene(Scene *scene);

int main() {
    int sampling = 250;
    cout << "Samples: ";
    cin >> sampling;

    Scene scene; // scene
    int width = 512, height = width; // width must be divisible by thread_count
    Renderer renderer(&scene, width, height, sampling, 64, 16, 0.25f, 0.000001f, 100.f);
    Camera camera(
        glm::vec3(0, 5, 15), // normal: 0, 5, 15 | top-down: 0, 15, 0
        (float)renderer.height / (float)renderer.width,
        180,
        Rotation(M_PI,0,0) // normal: M_PI,0,0 | top-down: M_PI/2,0,0
    ); // camera
    scene.setCamera(&camera);
    Skybox skybox(
        Color(63, 178, 232), Color(225, 244, 252), Color(225, 244, 252),
        true, Color(0, 0, 0)
    );
    scene.setSkybox(&skybox);
    setupScene(&scene);

    std::string path = img_output_path + getFileTimestamp() + ".png";
    int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    printf("Render started...\n    - %d threads\n    - %d samples\n    - %d max bounces\n    - %dx%d\n    - output: %s\n", renderer.thread_count, renderer.samples, renderer.recursion_depth, renderer.width, renderer.height, path.c_str());
    png::image<png::rgba_pixel> image(renderer.width, renderer.height);
    renderThreads(&renderer, &image);

    /* Apply Filter */
    //vector<vector<double>> filter = getGaussian(5, 5, 10.0);
    //image = applyFilter(image, filter, 2);

    image.write(path);
    int64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    printf("Rendered in %.2f seconds\n", (double(end) - double(start)) / 1000);
    return 0;
}

void setupScene(Scene *scene) {
    /** Lights **/
    auto light = Rect(glm::vec3(-1, 9.95, -1), glm::vec3(-1, 9.95, 1), glm::vec3(1, 9.95,1), glm::vec3(1, 9.95, -1), Material(0.0, 0.0, 40.0, Color(253, 173, 92)));
    scene->addObjects(light);

    /** Walls **/
    auto wall1 = Rect(glm::vec3(-5, 0, -5), glm::vec3(-5, 10, -5), glm::vec3(-5, 10, 5), glm::vec3(-5, 0, 5), Material(0.0, 1.0, 0.0, Color(175, 0, 0)));
    scene->addObjects(wall1); // left
    auto wall2 = Rect(glm::vec3(5, 0, -5), glm::vec3(5, 0, 5), glm::vec3(5, 10, 5),glm::vec3(5, 10, -5), Material(0.0, 1.0, 0.0, Color(0, 175, 0)));
    scene->addObjects(wall2); // right

    auto wall3 = Rect(glm::vec3(-5, 0, -5), glm::vec3(5, 0, -5), glm::vec3(5, 10, -5), glm::vec3(-5, 10, -5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
    scene->addObjects(wall3); // straight
    //auto wall4 = Rect(glm::vec3(-5, 0, 10), glm::vec3(-5, 10, 10), glm::vec3(5, 10, 10), glm::vec3(5, 0, 5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
    //scene->addObjects(wall4); // back

    /** Floor & Roof **/
    Plane floor = Plane(glm::vec3(0.0, 0, 0.0), glm::vec3(0.0, 1.0, 0.0), Material(0.0, 1.0, 0.0, Color(255.0, 255.0, 255.0)));
    scene->addObject(std::make_unique<Plane>(floor)); // plane
    //auto floor = Rect(glm::vec3(5, 0, -5), glm::vec3(-5, 0, -5), glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
    //scene->addObjects(floor); // tile floor
    auto roof = Rect(glm::vec3(-5,10, -5), glm::vec3(5, 10, -5), glm::vec3(5, 10, 5), glm::vec3(-5, 10, 5), Material(0.0, 1.0, 0.0, Color(175, 175, 175)));
    scene->addObjects(roof);

    /** Box 1 (RECT PRISM) **/
    const Material box1mat = Material(1.0, 0.0, 0.0, Color(255, 255, 255));
    auto box1wallleft = Rect(glm::vec3(-2.5, 0, 1), glm::vec3(-3.5, 0, -2), glm::vec3(-3.5, 6, -2),glm::vec3(-2.5, 6, 1), box1mat);
    scene->addObjects(box1wallleft); // left
    auto box1wallright = Rect(glm::vec3(0.5, 0, 0), glm::vec3(-0.5, 0, -3), glm::vec3(-0.5, 6, -3),glm::vec3(0.5, 6, 0), box1mat);
    scene->addObjects(box1wallright); // right
    auto box1wallfront = Rect(glm::vec3(-2.5, 0, 1), glm::vec3(0.5, 0, 0), glm::vec3(0.5, 6, 0), glm::vec3(-2.5, 6, 1), box1mat);
    scene->addObjects(box1wallfront); // front
    auto box1wallback = Rect(glm::vec3(-3.5, 0, -2), glm::vec3(-0.5, 0, -3), glm::vec3(-0.5, 6, -3), glm::vec3(-3.5, 6, -2), box1mat);
    scene->addObjects(box1wallback); // back
    auto box1wallroof = Rect(glm::vec3(-3.5, 6, -2), glm::vec3(-2.5, 6, 1), glm::vec3(0.5, 6, 0), glm::vec3(-0.5, 6, -3), box1mat);
    scene->addObjects(box1wallroof); // roof

    /** Box 2 (CUBE) **/
    const Material box2mat = Material(0.0, 1.0, 0.0, Color(255, 255, 255));
    auto box2wallleft = Rect(glm::vec3(0, 0, 3), glm::vec3(1, 0, 1.5), glm::vec3(1, 3, 1.5),glm::vec3(0, 3, 3), box2mat);
    scene->addObjects(box2wallleft); // left
    auto box2wallright = Rect(glm::vec3(3, 0, 4), glm::vec3(4, 0, 1), glm::vec3(4, 3, 1),glm::vec3(3, 3, 4), box2mat);
    scene->addObjects(box2wallright); // right
    auto box2wallfront = Rect(glm::vec3(0, 0, 3), glm::vec3(3, 0, 4), glm::vec3(3, 3, 4), glm::vec3(0, 3, 3), box2mat);
    scene->addObjects(box2wallfront); // front
    auto box2wallback = Rect(glm::vec3(1, 0, 0), glm::vec3(4, 0, 1), glm::vec3(4, 3, 1), glm::vec3(1, 3, 0), box2mat);
    scene->addObjects(box2wallback); // back
    auto box2wallroof = Rect(glm::vec3(1, 3, 0), glm::vec3(0, 3, 3), glm::vec3(3, 3, 4), glm::vec3(4, 3, 1), box2mat);
    scene->addObjects(box2wallroof); // roof


    /** Objects **/
    //std::string file_name = obj_path + "teddy.obj";
    //scene->loadObjectFile(file_name, Material(0.0f, 1.0f, 0.0f, Color(255, 0, 0)), glm::vec3(0.0, 1.95, -7.0), glm::vec3(0.2));
}
