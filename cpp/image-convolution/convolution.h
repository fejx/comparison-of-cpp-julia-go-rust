#ifndef IMAGE_CONVOLUTION_CONVOLUTION_H
#define IMAGE_CONVOLUTION_CONVOLUTION_H

#include "Image.h"
#include "Kernel.h"

void convolve(const Image& input, Image& output, const Kernel<>& kernel);

#endif //IMAGE_CONVOLUTION_CONVOLUTION_H
