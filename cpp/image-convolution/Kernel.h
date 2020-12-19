#ifndef IMAGE_CONVOLUTION_KERNEL_H
#define IMAGE_CONVOLUTION_KERNEL_H

#include <vector>
#include <cmath>

template<typename T = float>
class Kernel {
public:
	explicit Kernel(int radius) :
		radius(radius),
		width(radius * 2 + 1),
		totalSize(width * width),
		matrix(std::vector<T>()) {
		matrix.resize(totalSize);
	}

	inline const T& at(int x, int y) const {
		const int index = combineIndex(x, y);
		return matrix[index];
	}

	T& operator[](int index) {
		return matrix[index];
	}

	const T& operator[](int index) const {
		return matrix[index];
	}

	std::vector<T> matrix;
	const int radius;
	const int width;
	const int totalSize;

private:
	inline int combineIndex(int x, int y) const {
		return y * width + x;
	}
};

Kernel<> getBlurKernel(int radius);

#endif //IMAGE_CONVOLUTION_KERNEL_H
