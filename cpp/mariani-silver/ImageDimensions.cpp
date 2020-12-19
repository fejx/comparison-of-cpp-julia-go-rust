#include "ImageDimensions.h"

using namespace std;

ostream& operator<<(ostream& os, const ImageDimensions& image_dimensions) {
	return os
		<< "image size: " << image_dimensions.width << " x " << image_dimensions.height
		<< ", top left at " << image_dimensions.topLeft
		<< ", bottom right at " << image_dimensions.bottomRight
		<< endl;
}
