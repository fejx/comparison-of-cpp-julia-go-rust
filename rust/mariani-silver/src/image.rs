pub mod image {
	use std::fs::File;
	use std::io::Write;

	pub type Color = u8;
	pub type Pixel = [Color; 3];

	pub struct Image {
		pub width: usize,
		pub height: usize,
		pub total_pixels: usize,
		pub maximum_color_value: Color,
		pub pixels: Vec<Pixel>,
	}

	impl<'a> Image {
		pub fn new(width: usize, height: usize) -> Image {
			Image::new_with_custom_color(width, height, 255)
		}

		pub fn new_with_custom_color(width: usize, height: usize, maximum_color_value: Color) -> Image {
			let total_pixels = width * height;
			let mut pixels = Vec::<Pixel>::new();
			pixels.reserve_exact(total_pixels);
			for _ in 0..total_pixels {
				pixels.push([0u8, 0u8, 0u8]);
			}

			Image {
				width,
				height,
				total_pixels,
				maximum_color_value,
				pixels,
			}
		}

		pub fn write(&self, file_name: &str) -> Result<(), std::io::Error> {
			let mut file = File::create(file_name)?;

			let newline = &[b'\n'];
			let space = &[b' '];

			file.write_all("P6\n".as_bytes())?;
			let width = self.width.to_string();
			file.write_all(width.as_bytes())?;
			file.write_all(space)?;
			let height = self.height.to_string();
			file.write_all(height.as_bytes())?;
			file.write_all(newline)?;
			let maximum_color_value = self.maximum_color_value.to_string();
			file.write_all(maximum_color_value.as_bytes())?;
			file.write_all(newline)?;

			for pixel in self.pixels.iter() {
				file.write_all(pixel)?;
			}

			file.flush()?;

			Ok(())
		}

		pub fn index_mut_unsafe(&'a self, x: usize, y: usize) -> &'a mut Pixel {
			let i = self.combine_index(x, y);
			unsafe {
				let mut_pixels_ptr = self.pixels.as_ptr() as *mut [u8; 3];
				let mut_pixel = mut_pixels_ptr.offset(i as isize);
				return &mut *mut_pixel;
			}
		}

		fn combine_index(&self, x: usize, y: usize) -> usize {
			y * self.width + x
		}
	}
}