#ifndef MARIANI_SILVER_RECTANGLESPLITTER_H
#define MARIANI_SILVER_RECTANGLESPLITTER_H

#include <array>

#include "Rectangle.h"

template<unsigned int partCount>
class RectangleSplitter {
public:
	static std::array<Rectangle, partCount> split(const Rectangle& original) {
		return RectangleSplitter(original).split();
	}

private:
	explicit RectangleSplitter(const Rectangle& original)
		: original(original),
		  splitDirection(original.getHeight() > original.getWidth() ? horizontal : vertical) {
	}

	std::array<Rectangle, partCount> split() const {
		std::array<Rectangle, partCount> parts;

		const unsigned int splittingLength = getSplittingLength();
		const unsigned int splitLength = splittingLength / partCount;
		unsigned int remainder = splittingLength % partCount;
		unsigned int offset = 0;

		for (int i = 0; i < partCount; i++) {
			unsigned int currentSplitLength = splitLength;
			if (remainder > 0) {
				currentSplitLength++;
				remainder--;
			}
			Rectangle splitRectangle = splitWith(currentSplitLength, offset);
			parts[i] = splitRectangle;

			offset += currentSplitLength;
		}
		return parts;
	}

private:
	[[nodiscard]] unsigned int getSplittingLength() const {
		switch (splitDirection) {
			case horizontal:
				return original.getHeight();
			case vertical:
				return original.getWidth();
			default:
				throw std::exception("Unknown split direction");
		}
	}

	[[nodiscard]] Rectangle splitWith(unsigned int length, unsigned int offset) const {
		switch (splitDirection) {
			case horizontal:
				return Rectangle(
					original.getX(), original.getY() + offset,
					original.getWidth(), length
				);
			case vertical:
				return Rectangle(
					original.getX() + offset, original.getY(),
					length, original.getHeight()
				);
			default:
				throw std::exception("Unknown split direction");
		}
	}

	enum Direction {
		horizontal, vertical
	};

	const Rectangle& original;
	const Direction splitDirection;
};


#endif //MARIANI_SILVER_RECTANGLESPLITTER_H
