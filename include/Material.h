#pragma once

class Material {
public:
    float reflectivity;
    float diffuse;
    float emissive;
    Color color;

    Material();

    Material(float reflectivity, float diffuse, float emissive, Color color);
};
