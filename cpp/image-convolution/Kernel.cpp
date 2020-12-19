#include "Kernel.h"

using namespace std;

Kernel<> getBlurKernel(int radius) {
	Kernel<> kernel(radius);
	int numberOfValues = kernel.totalSize;
	double value = 1.0 / numberOfValues;
	for (int i = 0; i < numberOfValues; ++i)
		kernel[i] = value;
	return kernel;
}
