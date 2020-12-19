#ifndef MARIANI_SILVER_IMAGEGENERATOR_H
#define MARIANI_SILVER_IMAGEGENERATOR_H

#include <cassert>
#include <array>
#include <future>

#include "ImageDimensions.h"
#include "MandelbrotPixelCalculator.h"
#include "RectangleSplitter.h"
#include "ImageGenerationWorker.h"

template<int maximum_iterations>
class ImageGenerator {
public:
	void generate(Image& image, const ImageDimensions& dimensions) const {
		assertImageMatchesDimensions(image, dimensions);
		const Rectangle rectangle = rectangleFromDimensions(dimensions);
		std::deque<std::future<void>> tasks;
		std::mutex tasksLock;
		ImageGenerationWorker<maximum_iterations, 2>::generate(
			image,
			dimensions,
			pixelCalculator,
			rectangle,
			tasks,
			tasksLock
		);
		while (!tasks.empty()) {
			tasks.front().get();
			tasks.pop_front();
		}
	}

private:

	inline void assertImageMatchesDimensions(const Image& image, const ImageDimensions& dimensions) const {
		assert(image.height == dimensions.getHeight());
		assert(image.width == dimensions.getWidth());
	}

	static Rectangle rectangleFromDimensions(const ImageDimensions& dimensions) {
		return Rectangle(0, 0, dimensions.getWidth(), dimensions.getHeight());
	}

	const MandelbrotPixelCalculator<maximum_iterations> pixelCalculator;
};

#endif //MARIANI_SILVER_IMAGEGENERATOR_H
