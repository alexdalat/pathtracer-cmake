#include "Atrous.h"

void Atrous::ApplyFilter(
    int width, int height,
    std::vector<glm::vec3> &color, std::vector<glm::vec3> &normal, std::vector<glm::vec3> &position, std::vector<glm::vec3> &final, int epochs
) {
    std::vector<glm::vec3> input = color;
    for (int s = 0; s < epochs; s++) {
        int stepScale = static_cast<int>(pow(2.0, s));
        for(int c = 0; c < 3; c++) {
            for (int p = 0; p < width*height; p++) {
                final[p][c] += Atrous::FilterPixel(p, c, stepScale, width, height, input, normal, position);
            }
        }
        input = final;
        if(s != epochs-1) Atrous::multiplyArr(final, 0);
    }
}
float Atrous::FilterPixel(
    int idx, int color_index, int stepScale, int width, int height,
    std::vector<glm::vec3> &color, std::vector<glm::vec3> &normal, std::vector<glm::vec3> &position
) {
    float kernel[25] = { 1.0 / 256.0, 1.0 / 64.0, 3.0 / 128.0, 1.0 / 64.0, 1.0 / 256.0,
                         1.0 / 64.0, 1.0 / 16.0, 3.0 / 32.0, 1.0 / 16.0, 1.0 / 64.0,
                         3.0 / 128.0, 3.0 / 32.0, 9.0 / 64.0, 3.0 / 32.0, 3.0 / 128.0,
                         1.0 / 64.0, 1.0 / 16.0, 3.0 / 32.0, 1.0 / 16.0, 1.0 / 64.0,
                         1.0 / 256.0, 1.0 / 64.0, 3.0 / 128.0, 1.0 / 64.0, 1.0 / 256.0 };
    int step_radius = 2;
    float c_phi = 0.2f;
    float n_phi = 1.0f;
    float p_phi = 1.0f;
    glm::vec3 sum(0, 0, 0);
    float cum_w = 0.0f;
    for (int h = -step_radius; h < step_radius; h++) { // guassian blur strategy
        for (int w = -step_radius; w < step_radius; w++) {
            for (int k = 0; k < 25; k++) { // kernel size
                int offset = (h * stepScale) * width + (w * stepScale); // offset*step*stepwidth
                int nidx = idx + offset;
                nidx = std::min(std::max(nidx, 0), height * width);

                float tv = color[idx][color_index] - color[nidx][color_index];
                float c_w = std::min(exp(-(glm::dot(tv, tv)) / c_phi), 1.0f);

                glm::vec3 t = normal[idx] - normal[nidx];
                float dist2 = std::max(dot(t, t) / (stepScale * stepScale), 0.0f);
                float n_w = std::min(exp(-(dist2) / n_phi), 1.0f);

                t = position[idx] - position[nidx];
                float p_w = std::min(exp(-(glm::dot(t, t)) / p_phi), 1.0f);

                float weight = c_w * n_w * p_w;
                sum += color[nidx][color_index] * weight * kernel[k]; // c_tmp * weight * kernel[i]
                cum_w += weight * kernel[k];
            }
        }
    }
    float lum = 0.2126f * sum.x + 0.7152f * sum.y + 0.0722f * sum.z;
    return lum / cum_w;
}
void Atrous::multiplyArr(std::vector<glm::vec3> &arr, float f) {
    for(int i = 0; i < arr.size(); i++) {
        arr[i] *= f;
    }
}
