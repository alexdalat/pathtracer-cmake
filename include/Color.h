#pragma once

class Color {
public:
    float r, g, b;
    Color();
    Color(float nrgb);
    Color(float nr, float ng, float nb);
    void set(float nr, float ng, float nb);
    Color add(Color c);
    Color sub(Color c);
    Color add(float f);
    Color sub(float f);
    Color multiply(float f);
    Color multiply(Color c);
    Color divide(float f);
    Color divide(Color c);
    Color interpolate(Color c, float f);
    static Color average(std::vector<Color> colors);
    Color clamp();
    std::string toString();
};
