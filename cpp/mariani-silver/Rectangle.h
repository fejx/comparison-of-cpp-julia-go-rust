#ifndef MARIANI_SILVER_RECTANGLE_H
#define MARIANI_SILVER_RECTANGLE_H

#include <ostream>

class Rectangle {
public:
	Rectangle() : Rectangle(0, 0, 0, 0) {}

	Rectangle(
		const unsigned int x, const unsigned int y,
		const unsigned int width, const unsigned int height
	) :
		x(x), y(y),
		width(width), height(height) {}

	friend std::ostream& operator<<(std::ostream& os, const Rectangle& rectangle);

	[[nodiscard]] unsigned int getX() const {
		return x;
	}

	[[nodiscard]] unsigned int getY() const {
		return y;
	}

	[[nodiscard]] unsigned int getEndX() const {
		return x + width;
	}

	[[nodiscard]] unsigned int getEndY() const {
		return y + height;
	}

	[[nodiscard]] unsigned int getWidth() const {
		return width;
	}

	[[nodiscard]] unsigned int getHeight() const {
		return height;
	}

private:
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
};


#endif //MARIANI_SILVER_RECTANGLE_H
