#include <iostream>

#include "convolution.h"
#include "Stopwatch.h"
#include "benchmark_utilities.h"

using namespace std;

typedef Stopwatch::nanoseconds duration_t;

duration_t performRun(const Image& input, Image& output, const Kernel<>& kernel) {
	static Stopwatch stopwatch{};
	stopwatch.start();
	convolve(input, output, kernel);
	return stopwatch.stop();
}

void printRuntime(duration_t duration, const Image& image, const Kernel<>& kernel) {
	auto duration_seconds = chrono::duration_cast<chrono::duration<float>>(duration);
	cout
		<< kernel.radius << ";"
		<< image.width << "x" << image.height << ";"
		<< duration_seconds.count() << endl;
}

void benchmarkKernel(
	int repetitions,
	const Image& input,
	Image& output,
	const Kernel<>& kernel
) {
	duration_t duration;
	for (int i = 0; i < repetitions; ++i) {
		duration = performRun(input, output, kernel);
		printRuntime(duration, input, kernel);
	}
}

void benchmarkBlur(int repetitions, int radius, const Image& input, Image& output) {
	const Kernel<> blur = getBlurKernel(radius);
	benchmarkKernel(repetitions, input, output, blur);
}

void fullBenchmark(int repetitions, const Image& image) {
	Image output = Image::newWithEqualSize(image);

	benchmarkBlur(repetitions, 10, image, output);
}

void runSingle() {
	auto image = Image::fromFile("sun.ppm");
	Image output = Image::newWithEqualSize(image);
	const auto kernel = getBlurKernel(10);
	Stopwatch stopwatch;
	stopwatch.start();
	convolve(image, output, kernel);
	duration_t duration = stopwatch.stop();
	output.writeToFile("result.ppm");
	auto duration_seconds = chrono::duration_cast<chrono::duration<float>>(duration);
	cout << duration_seconds.count() << endl;
}

int main() {
//  runSingle();
//  return 0;

	trySetRealtime();

	fullBenchmark(
		10,
		Image::fromFile("sun.ppm")
	);
}
