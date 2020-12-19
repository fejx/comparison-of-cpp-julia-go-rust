pub mod image_dimensions {
	use crate::complex::complex::Complex;

	pub struct ImageDimensions {
		pub width: usize,
		pub height: usize,
		pub top_left: Complex,
		pub bottom_right: Complex,
		scale_x: f64,
		scale_y: f64,
	}

	impl ImageDimensions {
		pub fn new(
			width: usize, height: usize,
			top_left: Complex, bottom_right: Complex,
		) -> ImageDimensions {
			let scale_x = (bottom_right.real - top_left.real) / width as f64;
			let scale_y = (bottom_right.imaginary - top_left.imaginary) / height as f64;
			ImageDimensions {
				width,
				height,
				top_left,
				bottom_right,
				scale_x,
				scale_y,
			}
		}

		pub fn sample_to(&self, x: usize, y: usize) -> Complex {
			let scaled_x = x as f64 * self.scale_x + self.top_left.real;
			let scaled_y = y as f64 * self.scale_y - self.bottom_right.imaginary;
			Complex::new(scaled_x, scaled_y)
		}
	}
}