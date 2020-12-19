#include "Rectangle.h"

using namespace std;

ostream& operator<<(ostream& os, const Rectangle& rectangle) {
	return os
		<< "(" << rectangle.getX() << ", " << rectangle.getY() << "), "
		<< rectangle.getWidth() << " x " << rectangle.getHeight()
		<< endl;
}
