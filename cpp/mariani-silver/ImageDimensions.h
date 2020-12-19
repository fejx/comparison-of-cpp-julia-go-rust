#ifndef MARIANI_SILVER_IMAGE_DIMENSIONS_H
#define MARIANI_SILVER_IMAGE_DIMENSIONS_H

#include <ostream>

#include "Complex.h"

class ImageDimensions {
public:
	ImageDimensions(
		unsigned int width, unsigned int height,
		const Complex& topLeft, const Complex& bottomRight
	) :
		width(width), height(height),
		topLeft(topLeft), bottomRight(bottomRight),
		scaleX((bottomRight.getRealPart() - topLeft.getRealPart()) / width),
		scaleY((bottomRight.getImaginaryPart() - topLeft.getImaginaryPart()) / height) {}

	friend std::ostream& operator<<(std::ostream& os, const ImageDimensions& image_dimensions);

	[[nodiscard]] Complex sampleTo(const unsigned int x, const unsigned int y) const {
		float scaled_x = x * scaleX + topLeft.getRealPart();
		float scaled_y = y * scaleY - bottomRight.getImaginaryPart();
		return Complex(scaled_x, scaled_y);
	}

	[[nodiscard]] Complex sampleTo(const unsigned int index) const {
		return sampleTo(
			index % width,
			index / width
		);
	}

	[[nodiscard]] unsigned int getWidth() const {
		return width;
	}

	[[nodiscard]] unsigned int getHeight() const {
		return height;
	}

	[[nodiscard]] unsigned int getPixelCount() const {
		return width * height;
	}

private:
	const unsigned int width;
	const unsigned int height;
	Complex topLeft;
	Complex bottomRight;
	float scaleX;
	float scaleY;
};

#endif //MARIANI_SILVER_IMAGE_DIMENSIONS_H
