#include "Image.h"

using namespace std;

template<typename T = color_t>
istream& operator>>(istream& istream, Pixel<T>& pixel) {
	char value;
	for (auto& channel : pixel) {
		istream.read(&value, sizeof(color_t));
		channel = value;
	}
	return istream;
}

template<typename T = color_t>
ostream& operator<<(ostream& ostream, const Pixel<T>& pixel) {
	for (const auto& channel : pixel) {
		ostream << channel;
	}
	return ostream;
}

ostream& operator<<(ostream& ostream, const Image& image) {
	ostream
		<< "P6" << "\n"
		<< image.width << " "
		<< image.height << "\n"
		<< "255\n";
	for (const auto& pixel : image.pixels) {
		ostream << pixel;
	}
	return ostream;
}
