#ifndef PATHTRACER_CMAKE_RENDERHELPER_H
#define PATHTRACER_CMAKE_RENDERHELPER_H

#include "Renderer.h"

void renderChunk(int x0, int y0, int x1, int y1, Renderer *renderer, png::image<png::rgba_pixel> *image) {
    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            Color cols;
            float u, v;
            for (int s = 0; s < renderer->samples; s++) {
                if (renderer->samples > 1) {
                    u = ((float)x + (float)randomDouble()) / (float)renderer->width;
                    v = ((float)y + (float)randomDouble()) / (float)renderer->height;
                } else { // sampling = 1
                    u = ((float)x + 0.5f) / (float)renderer->width;
                    v = ((float)y + 0.5f) / (float)renderer->height;
                }
                Ray ray = renderer->scene->camera->getRay(u, v);

                Color traced_color = renderer->renderPixel(ray).multiply(255);
                cols = cols.add(traced_color);
            }
            Color col = cols.divide((float)renderer->samples).clamp();
            col.r = 255 * pow(col.r / 255, 1 / 2.2f);
            col.g = 255 * pow(col.g / 255, 1 / 2.2f); // gamma correction
            col.b = 255 * pow(col.b / 255, 1 / 2.2f);

            image->set_pixel(x, y, png::rgba_pixel((int)col.r, (int)col.g, (int)col.b, 255));
        }
    }
}

void renderThreads(Renderer *renderer, png::image<png::rgba_pixel> *image) {
    vector<thread> threads;
    int threadCount = renderer->thread_count;
    //renderChunk(0, 0, renderer->width, renderer->height, renderer, pixels);
    int iwidth = renderer->width / threadCount;
    for (int i = 0; i < threadCount; i++) {
        try {
            threads.emplace_back(renderChunk, i * iwidth, 0, i * iwidth + iwidth, renderer->height, renderer, image);
        } catch (const char *err) {
            cout << string(err);
        }
    }
    for (int i = 0; i < threadCount; i++) {
        try {
            threads[i].join();
        } catch (const char *err) {
            cout << string(err);
        }
    }
}

#endif //PATHTRACER_CMAKE_RENDERHELPER_H
