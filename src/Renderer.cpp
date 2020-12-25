using namespace std;

#include <iostream>
#include "Renderer.h"
#include "Data.h"

Renderer::Renderer(Scene *scene, int width, int height, int samples, int thread_count, int recursion_depth, float light_loss, float min_dist, float max_dist) {
    this->scene = scene;
    this->width = width;
    this->height = height;
    this->samples = samples;
    this->thread_count = thread_count;
    this->recursion_depth = recursion_depth;
    this->light_loss = 1-light_loss;
    this->min_dist = min_dist;
    this->max_dist = max_dist;
}

Color Renderer::renderPixel(Ray ray, int depth) {
    if(depth > this->recursion_depth) return Color(0);

    Intersection intersect = this->scene->castRay(&ray, this);
    if (!intersect.hit) {
        if (scene->skybox) return scene->skybox->getColorAt(ray.direction).divide(255);
        return Color(255,105,180).divide(255); // hot pink default
    }
    glm::vec3 normal = intersect.object->calculateNormal(intersect.collisionPoint, ray.direction);
    Material material = intersect.object->getMaterial();

    glm::vec3 reflectionDir = Ray::reflect(ray, normal);
    glm::vec3 diffuseDir = Ray::diffuse(normal);

    glm::vec3 ndir;
    if (material.diffuse > 0) ndir = diffuseDir;
    if (material.reflectivity > 0) ndir = reflectionDir;
    if (material.diffuse > 0 && material.reflectivity > 0 && randomDouble() < 0.5) ndir = diffuseDir;

    if (material.emissive <= 0)
        return this->renderPixel(Ray(intersect.collisionPoint, ndir), depth + 1).multiply(material.color.divide(255)).multiply(this->light_loss);
    else
        return material.color.divide(255).multiply(material.emissive);
}

void Renderer::SetScene(Scene &scene) {
	this->scene = &scene;
}