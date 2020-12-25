
#include "Filter.h"
#include <iostream>
#include <math.h>

#define M_PI 3.14159

png::image<png::rgba_pixel> applyFilter(png::image<png::rgba_pixel> &image, std::vector<std::vector<double>> &filter) {
    int height = image.get_height();
    int width = image.get_width();
    auto filterHeight = (int)filter.size();
    auto filterWidth = (int)filter[0].size();
    int newImageHeight = height-filterHeight+1;
    int newImageWidth = width-filterWidth+1;
    int i,j,h,w;

    png::image<png::rgba_pixel> newImage(newImageWidth, newImageHeight);
    for (i=0 ; i<newImageHeight ; i++) {
        for (j=0 ; j<newImageWidth ; j++) {
            for (h=i ; h<i+filterHeight ; h++) {
                for (w=j ; w<j+filterWidth ; w++) {
                    newImage[i][j].red += (int)filter[h-i][w-j]*image[h][w].red;
                    newImage[i][j].green += (int)filter[h-i][w-j]*image[h][w].green;
                    newImage[i][j].blue += (int)filter[h-i][w-j]*image[h][w].blue;
                    newImage[i][j].alpha = 255;
                }
            }
        }
    }

    return newImage;
}
png::image<png::rgba_pixel> applyFilter(png::image<png::rgba_pixel> &image, std::vector<std::vector<double>> &filter, int times) {
    png::image<png::rgba_pixel> newImage = image;
    for(int i=0 ; i<times ; i++) {
        newImage = applyFilter(newImage, filter);
    }
    return newImage;
}


std::vector<std::vector<double>> getGaussian(int height, int width, double sigma) {
    std::vector<std::vector<double>> kernel(height, std::vector<double>(width));
    double sum=0.0;
    int i,j;

    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            sum += kernel[i][j];
        }
    }

    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}