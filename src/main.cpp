
#include <iostream>
#include <string>
#include <cmath>
#include <future>

#include <glm/glm.hpp>

// project specific
#include "RenderHelper.h"
#include "Color.h"
#include "Plane.h"
#include "Util.h"
#include "Material.h"
#include "Camera.h"
#include "Skybox.h"
#include "Renderer.h"
#include "Scene.h"
#include "Rotation.h"
#include "Shapes.h"

// input paths
const std::string obj_path = "../../extra/objs/";
// output paths
const std::string img_dir = "../imgs/";
const std::string img_name = Util::getFileTimestamp();

void setupScene(Scene *scene);
template <typename T> T input(std::string var_name, T variable) {
    std::cout << var_name+std::string(" [default = ")+std::to_string(variable)+"]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ) {
        std::istringstream stream( input );
        stream >> variable;
    }
    return variable;
}
int main() {
    Renderer renderer;
    std::cout << "\n";
    renderer.setSamples(input<int>("Samples", 1));
    renderer.setWidth(input<int>("Width", 256));
    renderer.setHeight(input<int>("Height", renderer.width));
    renderer.setRecursionDepth(input<int>("Recursion", 1));
    renderer.setThreadCount(renderer.width / 16);
    renderer.setImageDir(img_dir);
    renderer.setImageName(img_name);
    renderer.setLightLoss(0.2f);

    Scene scene;
    renderer.setScene(&scene);
    Camera camera(
        glm::vec3(0, 3, 15), // normal: 0, 5, 15 | top-down: 0, 15, 0
        (float)renderer.height / (float)renderer.width,
        180,
        Rotation(0,0,0) // normal: 0,0,0 | top-down: -M_PI/2,0,0
    ); // camera
    scene.setCamera(&camera);
    Skybox skybox(
        Color(63, 178, 232), Color(225, 244, 252), Color(225, 244, 252),
        false, Color(0, 0, 0)
    );
    scene.setSkybox(&skybox);
    setupScene(&scene);

    Animation rotateAround;
    //renderer.setAnimation(&rotateAround);

    render(&renderer); // execute
    return 0;
}

void setupScene(Scene *scene) {
    /** Floor **/
    Plane floor = Plane(glm::vec3(0.0, 0, 0.0), glm::vec3(0.0, 1.0, 0.0), Material(0.0, 1.0, 0.0, Color(255.0, 255.0, 255.0)));
    scene->addObject(std::make_unique<Plane>(floor)); // plane
    //auto floor = Rect(glm::vec3(5, 0, -5), glm::vec3(-5, 0, -5), glm::vec3(-5, 0, 5), glm::vec3(5, 0, 5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
    //scene->addObjects(floor); // floor tile

    // Lights //
    //auto light = Rect(glm::vec3(-3, 9.95, -3), glm::vec3(-3, 9.95, -1), glm::vec3(-1, 9.95,-1), glm::vec3(-1, 9.95, -3), Material(0.0, 0.0, 40.0, Color(253, 173, 92)));
    //scene->addObjects(light);

    /*
    // Walls //
    auto wall1 = Rect(glm::vec3(-5, 0, -5), glm::vec3(-5, 10, -5), glm::vec3(-5, 10, 5), glm::vec3(-5, 0, 5), Material(0.0, 1.0, 0.0, Color(175, 0, 0)));
    scene->addObjects(wall1); // left
    auto wall2 = Rect(glm::vec3(5, 0, -5), glm::vec3(5, 0, 5), glm::vec3(5, 10, 5),glm::vec3(5, 10, -5), Material(0.0, 1.0, 0.0, Color(0, 175, 0)));
    scene->addObjects(wall2); // right

    auto wall3 = Rect(glm::vec3(-5, 0, -5), glm::vec3(5, 0, -5), glm::vec3(5, 10, -5), glm::vec3(-5, 10, -5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
    scene->addObjects(wall3); // straight
    //auto wall4 = Rect(glm::vec3(-5, 0, 10), glm::vec3(-5, 10, 10), glm::vec3(5, 10, 10), glm::vec3(5, 0, 5), Material(0.0, 1.0, 0.0, Color(255, 255, 255)));
    //scene->addObjects(wall4); // back

    // Roof //
    auto roof = Rect(glm::vec3(-5,10, -5), glm::vec3(5, 10, -5), glm::vec3(5, 10, 5), glm::vec3(-5, 10, 5), Material(0.0, 1.0, 0.0, Color(175, 175, 175)));
    scene->addObjects(roof);

    // Box 1 (RECT PRISM) //
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

    // Box 2 (CUBE) //
    const Material box2mat = Material(0.0, 1.0, 0.0, Color(255, 255, 255));
    auto box2wallleft = Rect(glm::vec3(0, 0, 3), glm::vec3(1, 0, 0), glm::vec3(1, 3, 0),glm::vec3(0, 3, 3), box2mat);
    scene->addObjects(box2wallleft); // left
    auto box2wallright = Rect(glm::vec3(3, 0, 4), glm::vec3(4, 0, 1), glm::vec3(4, 3, 1),glm::vec3(3, 3, 4), box2mat);
    scene->addObjects(box2wallright); // right
    auto box2wallfront = Rect(glm::vec3(0, 0, 3), glm::vec3(3, 0, 4), glm::vec3(3, 3, 4), glm::vec3(0, 3, 3), box2mat);
    scene->addObjects(box2wallfront); // front
    auto box2wallback = Rect(glm::vec3(1, 0, 0), glm::vec3(4, 0, 1), glm::vec3(4, 3, 1), glm::vec3(1, 3, 0), box2mat);
    scene->addObjects(box2wallback); // back
    auto box2wallroof = Rect(glm::vec3(1, 3, 0), glm::vec3(0, 3, 3), glm::vec3(3, 3, 4), glm::vec3(4, 3, 1), box2mat);
    scene->addObjects(box2wallroof); // roof
    */

    // Objects //
    std::string file_name = obj_path + "teddy.obj";
    scene->loadObjectFile(file_name, Material(0.0f, 1.0f, 0.0f, Color(255, 0, 0)), glm::vec3(0, 3, 0), glm::vec3(0.15));
}
