pub mod mandelbrot_pixel_calculation {
	use crate::complex::complex::Complex;
	use crate::image::image::Pixel;

	const MAXIMUM_ITERATIONS: usize = 255;

	pub struct MandelbrotPixelCalculator {
		lookup_table: [Pixel; MAXIMUM_ITERATIONS]
	}

	impl MandelbrotPixelCalculator {
		pub fn new() -> MandelbrotPixelCalculator {
			let hue_scale: f64 = 360.0 / MAXIMUM_ITERATIONS as f64;
			let mut lookup_table = [[0u8, 0u8, 0u8]; MAXIMUM_ITERATIONS];
			let mut i = 0usize;
			for pixel in lookup_table.iter_mut() {
				let scaled_value = (i as f64 * hue_scale) as usize;
				i += 1;
				*pixel = hsv_to_rgb(scaled_value, 1.0, 1.0);
			}
			lookup_table[MAXIMUM_ITERATIONS - 1] = [0u8, 0u8, 0u8];
			return MandelbrotPixelCalculator {
				lookup_table
			};
		}

		pub fn get_color_for(&self, c: Complex) -> Pixel {
			let iterations_when_diverging = get_iterations_when_diverging(c);
			self.lookup_table[iterations_when_diverging - 1]
		}
	}


	fn get_iterations_when_diverging(seed_number: Complex) -> usize {
		let mut last_value = Complex::new(0.0, 0.0);
		let mut i: usize = 1;
		while i < MAXIMUM_ITERATIONS {
			last_value.square();
			last_value += seed_number;
			if will_diverge(last_value) {
				break;
			}
			i += 1;
		}
		i
	}

	fn will_diverge(value: Complex) -> bool {
		value.absolute_squared() > 4.0
	}

	fn hsv_to_rgb(hue: usize, saturation: f64, value: f64) -> Pixel {
		let c = saturation * value;
		let x = c * (1.0 - (((hue as f64 / 60.0) % 2.0) - 1.0).abs());
		let m = value - c;
		let rs: f64;
		let gs: f64;
		let bs: f64;

		if hue < 60 {
			rs = c;
			gs = x;
			bs = 0.0;
		} else if hue >= 60 && hue < 120 {
			rs = x;
			gs = c;
			bs = 0.0;
		} else if hue >= 120 && hue < 180 {
			rs = 0.0;
			gs = c;
			bs = x;
		} else if hue >= 180 && hue < 240 {
			rs = 0.0;
			gs = x;
			bs = c;
		} else if hue >= 240 && hue < 300 {
			rs = x;
			gs = 0.0;
			bs = c;
		} else {
			rs = c;
			gs = 0.0;
			bs = x;
		}
		let red = ((rs + m) * 255.0) as u8;
		let green = ((gs + m) * 255.0) as u8;
		let blue = ((bs + m) * 255.0) as u8;
		[red, green, blue]
	}
}