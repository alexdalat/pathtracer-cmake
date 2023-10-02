
#include "Renderer.h"

Color Renderer::trace(Ray ray, int depth) {
    if(depth > this->recursion_depth) return Color(0);

    Intersection intersect = this->scene->castRay(&ray, this);
    if (!intersect.hit) {
        if (scene->skybox) return scene->skybox->getColorAt(ray.direction).divide(255);
        return Color(255,105,180).divide(255); // hot pink default
    }

    glm::vec3 normal = intersect.object->calculateNormal(intersect.point, ray.direction);
    Material material = intersect.object->getMaterial();

    glm::vec3 reflectionDir = Ray::reflect(ray, normal);
    glm::vec3 diffuseDir = Ray::diffuse(normal);

    glm::vec3 ndir;
    if (material.diffuse > 0) ndir = diffuseDir;
    if (material.reflectivity > 0) ndir = reflectionDir;
    if (material.diffuse > 0 && material.reflectivity > 0 && randomDouble() < 0.5) ndir = diffuseDir;

    if (material.emissive <= 0)
        return this->trace(Ray(intersect.point, ndir), depth + 1).multiply(material.color.divide(255)).multiply(1.f-this->light_loss);
    else
        return material.color.divide(255).multiply(material.emissive);
}
