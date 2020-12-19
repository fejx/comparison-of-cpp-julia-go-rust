#include <iostream>

#include "ImageGenerator.h"
#include "Stopwatch.h"
#include "benchmark_utilities.h"

using namespace std;

typedef Stopwatch::nanoseconds duration_t;

template<int maximum_iterations>
duration_t performRun(
	const ImageGenerator<maximum_iterations> generator,
	Image& image,
	const ImageDimensions& dimensions
) {
	static Stopwatch stopwatch;
	stopwatch.start();
	generator.generate(image, dimensions);
	return stopwatch.stop();
}

void printRuntime(duration_t duration, const Image& image) {
	auto duration_seconds = chrono::duration_cast<chrono::duration<float>>(duration);
	cout
		<< image.width << "x" << image.height << ";"
		<< duration_seconds.count()
		<< endl;
}

Image getImageFor(const ImageDimensions& dimensions) {
	return Image(dimensions.getWidth(), dimensions.getHeight());
}

template<int maximum_iterations>
void benchmarkDimensions(
	int repetitions,
	const ImageGenerator<maximum_iterations>& generator,
	const ImageDimensions& dimensions
) {
	duration_t duration;
	for (int i = 0; i < repetitions; ++i) {
		Image image = getImageFor(dimensions);
		duration = performRun(generator, image, dimensions);
		printRuntime(duration, image);
	}
}

ImageDimensions buildDimensions(unsigned int width, unsigned int height) {
	const Complex topLeft(-2.74529004, 1.23807502);
	const Complex bottomRight(1.25470996, -1.01192498);
	return ImageDimensions(
		width, height,
		topLeft, bottomRight
	);
}

void fullBenchmark(int repetitions) {
	const auto dimensions = buildDimensions(32000, 12000);
	ImageGenerator<255> generator;
	benchmarkDimensions(repetitions, generator, dimensions);
}

void runSingle() {
	ImageGenerator<255> generator;
	const auto dimensions = buildDimensions(320, 120);
	Image image = getImageFor(dimensions);
	generator.generate(image, dimensions);
	image.writeToFile("mandelbrot.ppm");
}

int main() {
//    runSingle();
//    return 0;

	trySetRealtime();

	fullBenchmark(10);
}
