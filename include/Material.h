#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

using namespace std;
#include "Color.h"

class Material {
public:
    float reflectivity;
    float diffuse;
    float emissive;
    Color color;

    Material();

    Material(float reflectivity, float diffuse, float emissive, Color color);
};

#endif