use std::sync::Arc;
use std::time::{Duration, Instant};

use crate::benchmark_utilities::benchmark_utilities::set_realtime;
use crate::complex::complex::Complex;
use crate::image::image::Image;
use crate::image_dimensions::image_dimensions::ImageDimensions;
use crate::image_generator::image_generator::generate;
use crate::mandelbrot_pixel_calculation::mandelbrot_pixel_calculation::MandelbrotPixelCalculator;

mod benchmark_utilities;
mod image_dimensions;
mod complex;
mod image;
mod mandelbrot_pixel_calculation;
mod image_generator;
mod rectangle;
mod rectangle_splitter;
mod wait_group;

fn main() {
	run_single();
	// set_realtime();
	// full_benchmark(10);
}

fn run_single() {
	let dimensions = Arc::new(build_dimensions(320, 120));
	let image = Arc::new(get_image_for(&dimensions));
	let generator = Arc::new(MandelbrotPixelCalculator::new());
	generate(generator, image.clone(), dimensions);
	image.write("result.ppm").unwrap();
}

fn get_image_for(dimensions: &ImageDimensions) -> Image {
	Image::new(dimensions.width, dimensions.height)
}

fn build_dimensions(width: usize, height: usize) -> ImageDimensions {
	ImageDimensions::new(
		width,
		height,
		Complex::new(-2.74529004, 1.23807502),
		Complex::new(1.25470996, -1.01192498),
	)
}

fn full_benchmark(repetitions: usize) {
	let dimensions = Arc::new(build_dimensions(32000, 12000));
	let calculator = Arc::new(MandelbrotPixelCalculator::new());
	benchmark_dimensions(repetitions, calculator, dimensions);
}

fn benchmark_dimensions(
	repetitions: usize,
	calculator: Arc<MandelbrotPixelCalculator>,
	dimensions: Arc<ImageDimensions>,
) {
	for _ in 0..repetitions {
		let image = Arc::new(get_image_for(dimensions.as_ref()));
		let duration = perform_run(
            calculator.clone(), dimensions.clone(), image.clone(),
        );
		print_runtime(duration, image.as_ref());
	}
}

fn print_runtime(duration: Duration, image: &Image) {
	println!("{}x{}:{}", image.width, image.height, duration.as_secs_f32())
}

fn perform_run(
    calculator: Arc<MandelbrotPixelCalculator>,
    dimensions: Arc<ImageDimensions>,
    image: Arc<Image>,
) -> Duration {
	let before = Instant::now();
	generate(calculator, image.clone(), dimensions);
	before.elapsed()
}
