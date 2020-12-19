#ifndef MARIANI_SILVER_IMAGEGENERATIONWORKER_H
#define MARIANI_SILVER_IMAGEGENERATIONWORKER_H

#include <future>
#include <optional>
#include <deque>
#include <mutex>

#include "Image.h"
#include "ImageDimensions.h"
#include "MandelbrotPixelCalculator.h"
#include "RectangleSplitter.h"

template<int maximum_iterations, unsigned int splitCount>
class ImageGenerationWorker {
public:
	static void generate(
		Image& image,
		const ImageDimensions& dimensions,
		const MandelbrotPixelCalculator<maximum_iterations>& pixelCalculator,
		const Rectangle& rectangle,
		std::deque<std::future<void>>& tasks,
		std::mutex& tasksLock
	) {
		ImageGenerationWorker(image, dimensions, pixelCalculator, rectangle, tasks, tasksLock).work();
	}

private:
	ImageGenerationWorker(
		Image& image,
		const ImageDimensions& dimensions,
		const MandelbrotPixelCalculator<maximum_iterations>& pixelCalculator,
		const Rectangle& rectangle,
		std::deque<std::future<void>>& tasks,
		std::mutex& tasksLock
	) :
		image(image),
		dimensions(dimensions),
		pixelCalculator(pixelCalculator),
		rectangle(rectangle),
		tasks(tasks),
		tasksLock(tasksLock) {}

	void work() {
		if (isTiny())
			generateWholeRectangle();
		else
			calculateOutlinesAndSplit();
	}

	bool isTiny() {
		return rectangle.getWidth() <= maxRectangleSize
		       && rectangle.getHeight() <= maxRectangleSize;
	}

	void calculateOutlinesAndSplit() {
		auto fillColor = generateOutlines();

		if (fillColor.has_value())
			fillWholeRectangle(fillColor.value());
		else
			split();
	}

	void generateWholeRectangle() {
		for (unsigned int x = rectangle.getX(); x < rectangle.getEndX(); x++)
			for (unsigned int y = rectangle.getY(); y < rectangle.getEndY(); y++) {
				generatePoint(x, y);
			}
	}

	void fillWholeRectangle(const Pixel<> color) {
		constexpr static const Pixel<> empty{};
		for (unsigned int x = rectangle.getX(); x < rectangle.getEndX(); x++)
			for (unsigned int y = rectangle.getY(); y < rectangle.getEndY(); y++) {
				image.at(x, y) = color;
			}
	}

	void split() {
		const auto parts = RectangleSplitter<splitCount>::split(rectangle);
		std::lock_guard guard(tasksLock);
		for (int i = 0; i < splitCount; ++i) {
			tasks.push_back(std::async(
				ImageGenerationWorker::generate,
				std::ref(image),
				std::ref(dimensions),
				std::ref(pixelCalculator),
				parts[i],
				std::ref(tasks),
				std::ref(tasksLock)
			));
		}
	}

	std::optional<std::reference_wrapper<const Pixel<>>> generateOutlines() {
		unsigned int topY = rectangle.getY();
		unsigned int bottomY = topY + rectangle.getHeight() - 1;
		unsigned int leftX = rectangle.getX();
		unsigned int rightX = leftX + rectangle.getWidth() - 1;

		bool outlinesIdentical = true;
		auto referenceColor = generatePoint(leftX, topY);

		for (unsigned int x = leftX; x <= rightX; x++) {
			checkOutlinePixel(x, topY, referenceColor, outlinesIdentical);
			checkOutlinePixel(x, bottomY, referenceColor, outlinesIdentical);
		}
		for (unsigned int y = topY; y <= bottomY; y++) {
			checkOutlinePixel(leftX, y, referenceColor, outlinesIdentical);
			checkOutlinePixel(rightX, y, referenceColor, outlinesIdentical);
		}

		if (outlinesIdentical)
			return std::cref(referenceColor);
		else
			return {};
	}

	void checkOutlinePixel(unsigned int x, unsigned int y, const Pixel<>& referenceColor, bool& outlinesIdentical) {
		const auto currentColor = generatePoint(x, y);
		if (currentColor != referenceColor)
			outlinesIdentical = false;
	}


	const Pixel<>& generatePoint(unsigned int x, unsigned int y) { // NOLINT(modernize-use-nodiscard)
		constexpr static const Pixel<> empty{};
		Pixel<>& target = image.at(x, y);
		if (target != empty)
			return target;
		const Complex sampled = dimensions.sampleTo(x, y);
		const Pixel<>& color = pixelCalculator.getColorFor(sampled);
		target = color;
		return color;
	}

	Image& image;
	const ImageDimensions& dimensions;
	const MandelbrotPixelCalculator<maximum_iterations> pixelCalculator;
	const Rectangle rectangle;
	const unsigned int maxRectangleSize = 50;
	std::deque<std::future<void>>& tasks;
	std::mutex& tasksLock;
};

#endif //MARIANI_SILVER_IMAGEGENERATIONWORKER_H
