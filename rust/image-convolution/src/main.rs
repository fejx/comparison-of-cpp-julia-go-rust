use std::sync::Arc;
use std::time::{Duration, Instant};

use benchmark_utilities::benchmark_utilities::set_realtime;

use crate::convolution::convolution::convolve;
use crate::image::image::Image;
use crate::kernel::kernel::Kernel;

mod convolution;
mod kernel;
mod image;
mod benchmark_utilities;

fn main() {
	// run_single();
	set_realtime();
	full_benchmark(10, "sun.ppm");
}

fn run_single() {
	let input = Image::new_from_file("test.ppm").unwrap();
	let mut output = Arc::new(Image::new_with_equal_size(&input));
	let kernel = Kernel::new_blur(2);
	convolve(Arc::new(input), output.clone(), Arc::new(kernel));
	output.write("result.ppm").unwrap();
}

fn full_benchmark(repetitions: usize, file_name: &str) {
	let input = Arc::new(Image::new_from_file(file_name).unwrap());
	let mut output = Arc::new(Image::new_with_equal_size(&input));

	benchmark_blur(repetitions, 10, input.clone(), output.clone());
}

fn benchmark_blur(repetitions: usize, radius: usize, input: Arc<Image>, output: Arc<Image>) {
	let kernel = Arc::new(Kernel::new_blur(radius));
	benchmark_kernel(repetitions, input, output, kernel);
}

fn benchmark_kernel(repetitions: usize, input: Arc<Image>, output: Arc<Image>, kernel: Arc<Kernel>) {
	for _ in 0..repetitions {
		let duration = perform_run(input.clone(), output.clone(), kernel.clone());
		print_runtime(duration, &input, &kernel);
	}
}

fn print_runtime(duration: Duration, image: &Image, kernel: &Kernel) {
	println!("{};{}x{}:{}", kernel.radius, image.width, image.height, duration.as_secs_f32())
}

fn perform_run(
	input: Arc<Image>, output: Arc<Image>, kernel: Arc<Kernel>
) -> Duration {
	let before = Instant::now();
	convolve(input, output, kernel);
	before.elapsed()
}
