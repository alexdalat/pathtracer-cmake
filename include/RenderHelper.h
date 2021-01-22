#ifndef PATHTRACER_CMAKE_RENDERHELPER_H
#define PATHTRACER_CMAKE_RENDERHELPER_H

#include "Renderer.h"
#include "Atrous.h"

void renderThreads(Renderer *renderer, png::image<png::rgba_pixel> *image);
void drawArray(std::vector<glm::vec3> &arr, std::string img_path, int width, int height);

void renderChunk(int x0, int y0, int x1, int y1, int thread_num, int thread_total, Renderer *renderer, png::image<png::rgba_pixel> *image) {
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
                Color traced_color = renderer->trace(ray).multiply(255);
                cols = cols.add(traced_color);
            }
            Color col = cols.divide((float)renderer->samples).clamp();
            col.r = 255 * pow(col.r / 255, 1 / 2.2f);
            col.g = 255 * pow(col.g / 255, 1 / 2.2f); // gamma correction
            col.b = 255 * pow(col.b / 255, 1 / 2.2f);
            image->set_pixel(x, y, png::rgba_pixel((int)col.r, (int)col.g, (int)col.b, 255));


            if(renderer->atrous > 0) {
                int idx = x + renderer->width * y;
                renderer->C[idx] = glm::vec3(col.r, col.g, col.b)/255.0f;
                u = ((float)x + 0.5f) / (float)renderer->width;
                v = ((float)y + 0.5f) / (float)renderer->height;
                Ray ray = renderer->scene->camera->getRay(u, v);
                Intersection intersect = renderer->scene->castRay(&ray, renderer);
                if(!intersect.hit) {
                    renderer->N[idx] = glm::vec3(-1, -1, -1);
                    renderer->P[idx] = glm::vec3(-1, -1, -1);
                } else {
                    glm::vec3 normal = intersect.object->calculateNormal(intersect.point, ray.direction);
                    renderer->N[idx] = normal;
                    renderer->P[idx] = intersect.point;
                }
            }
        }
    }
    printf("Thread %d / %d complete.\n", thread_num, thread_total);
}

void render(Renderer *renderer) {
    png::image<png::rgba_pixel> image(renderer->width, renderer->height);
    int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    printf("Render started...\n    - %d threads\n    - %d samples\n    - %d max bounces\n    - %dx%d\n    - output: %s\n\n", renderer->thread_count, renderer->samples, renderer->recursion_depth, renderer->width, renderer->height, renderer->img_path.c_str());
    renderThreads(renderer, &image);
    int64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    printf("Rendered in %.2f seconds\n\n", (double(end) - double(start)) / 1000);
    image.write(renderer->img_path+".png");

    if(renderer->atrous > 0) {
        printf("A-Trous denoising started...\n");
        int64_t start2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        std::vector<glm::vec3> F(renderer->width*renderer->height);
        for(int x = 0; x < F.size(); x++)
            F[x] = glm::vec3(0);
        Atrous::ApplyFilter(renderer->width, renderer->height, renderer->C, renderer->N, renderer->P, F, renderer->atrous);
        Atrous::multiplyArr(F, 255);
        drawArray(F, renderer->img_path+"-atrous.png", renderer->width, renderer->height);

        int64_t end2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        printf("Denoised in %.2f seconds\n", (double(end2) - double(start2)) / 1000);
    }
}



void drawArray(std::vector<glm::vec3> &arr, std::string img_path, int width, int height) {
    png::image< png::rgb_pixel > final_image(width, height);
    int p = 0; // index
    for(int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            final_image.set_pixel(x, y, png::rgb_pixel(static_cast<png::byte>(arr[p].x), static_cast<png::byte>(arr[p].y), static_cast<png::byte>(arr[p].z)));
            p++;
        }
    }
    final_image.write(img_path);
}

void renderThreads(Renderer *renderer, png::image<png::rgba_pixel> *image) {
    vector<thread> threads;
    int threadCount = renderer->thread_count;
    int iwidth = renderer->width / threadCount;
    for (int i = 0; i < threadCount; i++) {
        try {
            threads.emplace_back(renderChunk, i * iwidth, 0, i * iwidth + iwidth, renderer->height, i, threadCount, renderer, image);
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
