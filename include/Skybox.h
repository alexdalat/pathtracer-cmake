#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

using namespace std;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Color.h"

class Skybox {
public:
	glm::vec3 up;
	Color topColor;
	Color sideColor;
	Color bottomColor;
	bool override;
	Color overrideColor;
    float intensity;

    Skybox() {
        this->up = glm::vec3(0, 1, 0);
        this->topColor = Color(255.0f, 20.0f, 147.0f);
        this->sideColor = Color(255.0f, 20.0f, 147.0f);
        this->bottomColor = Color(255.0f, 255.0f, 255.0f);
        this->override = false;
        this->overrideColor = Color(255.0f, 20.0f, 147.0f);
        this->intensity = 1;
    }

    Skybox(Color topColor, Color sideColor, Color bottomColor, bool override = false,
           Color overrideColor = Color(255.0f, 20.0f, 147.0f), float intensity = 1, glm::vec3 up = glm::vec3(0, 1, 0));

    Color getColorAt(glm::vec3 dir);
};

#endif