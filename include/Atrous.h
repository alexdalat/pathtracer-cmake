#ifndef PATHTRACER_CMAKE_ATROUS_H
#define PATHTRACER_CMAKE_ATROUS_H

#include "Filter.h"

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

class Atrous : public Filter {
public:
    static void ApplyFilter(int width, int height, std::vector<glm::vec3> &color, std::vector<glm::vec3> &normal, std::vector<glm::vec3> &position, std::vector<glm::vec3> &final, int epochs = 3);
    static void multiplyArr(std::vector<glm::vec3> &arr, float f);

private:
    static float FilterPixel(int idx, int color_index, int stepScale, int width, int height, std::vector<glm::vec3> &color, std::vector<glm::vec3> &normal, std::vector<glm::vec3> &position);
};

#endif //PATHTRACER_CMAKE_ATROUS_H
