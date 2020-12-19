#include "convolution.h"

#include <thread>

using namespace std;

color_t roundToColor(const float& value) {
	return value + .5;
}

const color_t& clamp(const color_t& value, const color_t& min, const color_t& max) {
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

void convolveWorker(
	const Image& input, Image& output, const Kernel<>& kernel, int xOffset, int xSkip
) {
	const static Pixel<> FALLBACK{};
	int radius = kernel.radius;
	color_t maximumColorValue = input.maximumColorValue;
	Pixel<float> unroundedPixel;
	// Iterate over image
	for (int x = xOffset; x < input.width; x += xSkip) {
		for (int y = 0; y < input.height; ++y) {
			unroundedPixel.fill(0);

			// Iterate over kernel
			for (int ox = -radius; ox <= radius; ++ox) {
				for (int oy = -radius; oy <= radius; ++oy) {
					const float kernelValue = kernel.at(ox + radius, oy + radius);
					const Pixel<>& imageValue = input.atOr(x + ox, y + oy, FALLBACK);
					// Iterate over channels
					for (int channel = 0; channel < 3; ++channel) {
						unroundedPixel[channel] += kernelValue * imageValue[channel];
					}
				}
			}

			Pixel<>& outputPixel = output.at(x, y);
			for (int channel = 0; channel < 3; ++channel) {
				const color_t rounded = roundToColor(unroundedPixel[channel]);
				outputPixel[channel] = clamp(
					rounded, static_cast<color_t>(0), maximumColorValue
				);
			}
		}
	}
}

void convolve(const Image& input, Image& output, const Kernel<>& kernel) {
	unsigned int workerCount = std::thread::hardware_concurrency();

	vector<thread> threads;
	threads.reserve(workerCount);
	for (unsigned int i = 0; i < workerCount; i++)
		threads.emplace_back(
			convolveWorker,
			cref(input),
			ref(output),
			cref(kernel),
			i,
			workerCount
		);
	for (thread& thread : threads)
		thread.join();
}
