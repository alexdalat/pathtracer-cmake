//
// Created by xelada on 2/26/20.
//

#ifndef PATHTRACER_FILTER_H
#define PATHTRACER_FILTER_H

#include <vector>
#include <png++/image.hpp>
#include "Color.h"

png::image<png::rgba_pixel> applyFilter(png::image<png::rgba_pixel> &image, std::vector<std::vector<double>> &filter);
png::image<png::rgba_pixel> applyFilter(png::image<png::rgba_pixel> &image, std::vector<std::vector<double>> &filter, int times);

std::vector<std::vector<double>> getGaussian(int height, int width, double sigma);


#endif //PATHTRACER_FILTER_H
