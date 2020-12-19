#ifndef MARIANI_SILVER_MANDELBROT_PIXEL_CALCULATOR_H
#define MARIANI_SILVER_MANDELBROT_PIXEL_CALCULATOR_H

#include <cmath>
#include <array>

#include "Image.h"
#include "Complex.h"

template<int maximum_iterations>
class MandelbrotPixelCalculator {
private:
	typedef Pixel<> Pixel;
	typedef std::array<Pixel, maximum_iterations> lookupTable_t;
public:
	MandelbrotPixelCalculator() {
		buildLookupTable();
	}

	[[nodiscard]] const Pixel& getColorFor(const Complex& c) const {
		const unsigned int iterations_when_diverging = getIterationsWhenDiverging(c);
		return lookupTable[iterations_when_diverging - 1];
	}

	[[nodiscard]] constexpr unsigned int getMaximumIterations() const {
		return maximum_iterations;
	}

private:

	[[nodiscard]] unsigned int getIterationsWhenDiverging(const Complex& seed_number) const {
		Complex last_value = 0;
		unsigned int i = 1;
		while (i < maximum_iterations) {
			last_value.square();
			last_value += seed_number;
			if (willDiverge(last_value))
				break;
			i++;
		}
		return i;
	}

	[[nodiscard]] bool willDiverge(const Complex& sequence_entry) const {
		return sequence_entry.absolute_squared() > 4;
	}

	void buildLookupTable() {
		const size_t length = lookupTable.size();
		const float hue_scale = 360.0 / length;
		unsigned int i = 0;
		for (auto& pixel : lookupTable) {
			const unsigned int scaled_value = i * hue_scale;
			i++;
			pixel = HSVtoRGB(scaled_value, 1, 1);
		}
		lookupTable[maximum_iterations - 1] = {0, 0, 0};
	}

	// From https://gist.github.com/kuathadianto/200148f53616cbd226d993b400214a7f
	[[nodiscard]] Pixel HSVtoRGB(unsigned int hue, double saturation, double value) const {
		double C = saturation * value;
		double X = C * (1 - std::abs(std::fmod(hue / 60.0, 2) - 1));
		double m = value - C;
		double Rs, Gs, Bs;

		if (hue >= 0 && hue < 60) {
			Rs = C;
			Gs = X;
			Bs = 0;
		} else if (hue >= 60 && hue < 120) {
			Rs = X;
			Gs = C;
			Bs = 0;
		} else if (hue >= 120 && hue < 180) {
			Rs = 0;
			Gs = C;
			Bs = X;
		} else if (hue >= 180 && hue < 240) {
			Rs = 0;
			Gs = X;
			Bs = C;
		} else if (hue >= 240 && hue < 300) {
			Rs = X;
			Gs = 0;
			Bs = C;
		} else {
			Rs = C;
			Gs = 0;
			Bs = X;
		}
		const color_t red_part = (Rs + m) * 255;
		const color_t green_part = (Gs + m) * 255;
		const color_t blue_part = (Bs + m) * 255;
		return {red_part, green_part, blue_part};
	}

public:
	lookupTable_t lookupTable;
};

#endif //MARIANI_SILVER_MANDELBROT_PIXEL_CALCULATOR_H
