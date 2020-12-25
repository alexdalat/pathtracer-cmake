#ifndef PATHTRACER_CMAKE_SHAPES_H
#define PATHTRACER_CMAKE_SHAPES_H

static vector<unique_ptr<Object>> Rect(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, Material mat) {
    vector<unique_ptr<Object>> list;
    list.emplace_back(make_unique<Triangle>(p1, p2, p3, mat));
    list.emplace_back(make_unique<Triangle>(p3, p4, p1, mat));
    return list;
}

static vector<unique_ptr<Object>> RectPrism(glm::vec3 v1, glm::vec3 v2, Material mat) {
    vector<unique_ptr<Object>> list;
    // add 6 rects to list...
    return list;
}

#endif //PATHTRACER_CMAKE_SHAPES_H
