#pragma once

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
    if(renderer->animation == nullptr) printf("Thread %d / %d complete.\n", thread_num, thread_total);
}

void render(Renderer *renderer) {
    int iterations = 1;
    if (!std::filesystem::is_directory(renderer->img_dir) || !std::filesystem::exists(renderer->img_dir)) { std::filesystem::create_directory(renderer->img_dir); } // make imgs dir
    if(renderer->animation != nullptr) {
        iterations = renderer->animation->frames;
        renderer->img_dir = renderer->img_dir + renderer->img_name + "/";
        std::filesystem::create_directory(renderer->img_dir); // create folder
    }

    for(int i = 0; i < iterations; i++) {
        png::image<png::rgba_pixel> image(renderer->width, renderer->height);

        int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        if(i == 0) printf("Render %d started...\n    - %d threads\n    - %d samples\n    - %d max bounces\n    - %dx%d\n    - output: %s%s\n\n",
               i, renderer->thread_count, renderer->samples, renderer->recursion_depth, renderer->width, renderer->height,
               renderer->img_dir.c_str(), renderer->img_name.c_str());
        if(renderer->animation != nullptr) renderer->animation->update(renderer->scene, i); // animation step

        renderThreads(renderer, &image); // RENDERRRR

        int64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        printf("Rendered frame %d in %.2f seconds\n", i, (double(end) - double(start)) / 1000);

        image.write(renderer->img_dir+renderer->img_name+"-"+std::to_string(i)+".png");
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
