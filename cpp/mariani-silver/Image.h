#ifndef MARIANI_SILVER_IMAGE_H
#define MARIANI_SILVER_IMAGE_H

#include <ostream>
#include <vector>
#include <array>
#include <cstdint>
#include <fstream>

typedef uint8_t color_t;

template<typename T = color_t>
struct Pixel : public std::array<T, 3> {
	template<typename T2>
	friend std::istream& operator>>(std::istream& istream, Pixel<T2>& pixel);

	template<typename T2>
	friend std::ostream& operator<<(std::ostream& ostream, const Pixel<T2>& pixel);
};

class Image {
public:
	friend std::ostream& operator<<(std::ostream& ostream, const Image& image);

	Image(unsigned int width, unsigned int height) :
		width(width),
		height(height),
		totalSize(width * height),
		pixels(std::vector<Pixel<>>(totalSize)) {
	}

	void writeToFile(const std::string& filename) const {
		std::ofstream outputFile;
		outputFile.open(filename, std::ios::out | std::ios::binary);
		outputFile << *this;
	}

	[[nodiscard]] inline Pixel<>& at(unsigned int index) {
		return pixels[index];
	}

	[[nodiscard]] inline const Pixel<>& at(unsigned int index) const {
		return pixels[index];
	}

	[[nodiscard]] inline Pixel<>& at(unsigned int x, unsigned int y) {
		const unsigned int i = combineIndex(x, y);
		return pixels[i];
	}

	[[nodiscard]] inline const Pixel<>& at(unsigned int x, unsigned int y) const {
		const int i = combineIndex(x, y);
		return pixels[i];
	}

	unsigned int width, height, totalSize;
	std::vector<Pixel<>> pixels;

private:
	[[nodiscard]] inline unsigned int combineIndex(unsigned int x, unsigned int y) const {
		return y * width + x;
	}
};


#endif //MARIANI_SILVER_IMAGE_H
