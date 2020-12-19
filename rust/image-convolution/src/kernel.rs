pub mod kernel {
	pub struct Kernel {
		pub radius: usize,
		pub width: usize,
		pub total_size: usize,
		pub matrix: Vec<f32>,
	}

	impl Kernel {
		pub fn new(radius: usize) -> Kernel {
			let width = radius * 2 + 1;
			let total_size = width * width;
			let mut matrix = Vec::<f32>::new();
			matrix.reserve_exact(total_size);
			Kernel {
				radius,
				width,
				total_size,
				matrix,
			}
		}

		pub fn new_blur(radius: usize) -> Kernel {
			let mut kernel = Kernel::new(radius);
			let number_of_values = kernel.total_size;
			let value = 1f32 / number_of_values as f32;
			for _ in 0..kernel.total_size {
				kernel.matrix.push(value);
			}
			kernel
		}

		pub fn get(&self, x: usize, y: usize) -> f32 {
			let i = self.combine_index(x, y);
			*self.matrix.get(i).unwrap()
		}

		fn combine_index(&self, x: usize, y: usize) -> usize {
			y * self.width + x
		}
	}
}
