#ifndef IMAGE_CONVOLUTION_IMAGE_H
#define IMAGE_CONVOLUTION_IMAGE_H

#include <ostream>
#include <vector>
#include <array>
#include <cstdint>
#include <fstream>

typedef uint8_t color_t;

template<typename T = color_t>
struct Pixel : public std::array<T, 3> {
	friend std::istream& operator>>(std::istream& istream, Pixel<T>& pixel) {
		char value;
		for (auto& channel : pixel) {
			istream.read(&value, sizeof(color_t));
			channel = value;
		}
		return istream;
	}

	friend std::ostream& operator<<(std::ostream& ostream, const Pixel<T>& pixel) {
		for (const auto& channel : pixel) {
			ostream << channel;
		}
		return ostream;
	}
};

class Image {
public:
	friend std::ostream& operator<<(std::ostream& ostream, const Image& image) {
		ostream
			<< "P6" << "\n"
			<< image.width << " "
			<< image.height << "\n"
			<< image.maximumColorValue << "\n";
		for (const auto& pixel : image.pixels) {
			ostream << pixel;
		}
		return ostream;
	}

	static Image fromFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (file.is_open()) {
			return Image(file);
		} else throw std::exception();
	}

	explicit Image(std::istream& inputStream) {
		std::string header;
		inputStream >> header;
		if (header != "P6")
			throw std::exception();
		inputStream >> width >> height;
		totalSize = width * height;
		inputStream >> maximumColorValue;
		if (maximumColorValue > 255)
			throw std::exception();
		inputStream.get(); // skip the trailing white space
		pixels = std::vector<Pixel<>>(totalSize);
		for (auto& pixel : pixels) {
			inputStream >> pixel;
		}
	}

	Image(int width, int height, int maximumColorValue) :
		width(width),
		height(height),
		totalSize(width * height),
		maximumColorValue(maximumColorValue),
		pixels(std::vector<Pixel<>>(totalSize)) {
	}

	static Image newWithEqualSize(const Image& image) {
		Image newImage(image.width, image.height, image.maximumColorValue);
		return newImage;
	}

	void writeToFile(const std::string& filename) {
		std::ofstream outputFile;
		outputFile.open(filename, std::ios::out | std::ios::binary);
		outputFile << *this;
	}

	inline Pixel<>& at(int x, int y) {
		const int i = combineIndex(x, y);
		return pixels[i];
	}

	inline const Pixel<>& at(int x, int y) const {
		const int i = combineIndex(x, y);
		return pixels[i];
	}

	inline const Pixel<>& atOr(int x, int y, const Pixel<>& fallback) const {
		return (x < 0 || x >= width || y < 0 || y >= height)
		       ? fallback
		       : at(x, y);
	}

	int width, height, totalSize;
	int maximumColorValue;
	std::vector<Pixel<>> pixels;

private:
	inline int combineIndex(int x, int y) const {
		return y * width + x;
	}
};

#endif //IMAGE_CONVOLUTION_IMAGE_H
