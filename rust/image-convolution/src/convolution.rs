pub mod convolution {
	use std::sync::Arc;
	use std::thread;

	use crate::image::image::Image;
	use crate::kernel::kernel::Kernel;

	pub fn convolve(input: Arc<Image>, output: Arc<Image>, kernel: Arc<Kernel>) {
		let worker_count = num_cpus::get();
		let mut handles = Vec::new();
		handles.reserve_exact(worker_count);
		for i in 0..worker_count {
			let input = input.clone();
			let output = output.clone();
			let kernel = kernel.clone();
			let handle = thread::spawn(move || {
				convolve_worker(input, output, kernel, i, worker_count);
			});
			handles.push(handle);
		}
		for handle in handles {
			handle.join().unwrap();
		}
	}

	fn convolve_worker(
		input: Arc<Image>,
		output: Arc<Image>,
		kernel: Arc<Kernel>,
		x_offset: usize,
		x_skip: usize,
	) {
		let fallback = [0, 0, 0];
		let radius = kernel.radius as isize;
		let mut unrounded_pixel: [f32; 3];
		let maximum_color_value = input.maximum_color_value as f32;
		for x in (x_offset..input.width).step_by(x_skip) {
			for y in 0usize..input.height {
				unrounded_pixel = [0f32, 0f32, 0f32];

				for ox in -radius..=radius {
					for oy in -radius..=radius {
						let kernel_value = kernel.get((ox + radius) as usize, (oy + radius) as usize);
						let image_value = input
							.get(x as isize + ox, y as isize + oy)
							.unwrap_or(&fallback);
						for channel in 0..3 {
							unrounded_pixel[channel] += kernel_value * image_value[channel] as f32;
						}
					}
				}

				let pixel = output.index_mut_unsafe(x, y);
				for channel in 0..3 {
					let unrounded = unrounded_pixel[channel];
					let rounded = unrounded.round();
					let clamped = clamp(rounded, 0f32, maximum_color_value);
					let casted = clamped as u8;
					pixel[channel] = casted;
				}
			}
		}
	}

	fn clamp<T>(value: T, min: T, max: T) -> T where T: PartialOrd {
		if value > max {
			return max;
		} else if value < min {
			return min;
		} else {
			return value;
		}
	}
}