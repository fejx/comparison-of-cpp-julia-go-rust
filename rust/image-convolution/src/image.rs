pub mod image {
	use std::fs::File;
	use std::io::{BufRead, BufReader, ErrorKind, Read, Write};
	use std::ops::IndexMut;
	use std::error::Error;

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
		pub fn new(width: usize, height: usize, maximum_color_value: Color) -> Image {
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

		pub fn new_with_equal_size(image: &Image) -> Image {
			Image::new(image.width, image.height, image.maximum_color_value)
		}

		pub fn new_from_file(file_name: &str) -> Result<Image, Box<dyn Error>> {
			let file = File::open(file_name)?;
			let mut reader = BufReader::new(file);
			let mut buffer = String::new();

			// Header
			reader.read_line(&mut buffer)?;
			// Remove newline char
			buffer.pop();
			if &buffer != "P6" {
				return boxed_error("not a ppm file");
			}
			buffer.clear();

			// Width
			unsafe {
				reader.read_until(b' ', buffer.as_mut_vec())?;
			}
			let width = buffer.trim().parse::<usize>()?;
			buffer.clear();

			// Height
			reader.read_line(&mut buffer)?;
			let height = buffer.trim().parse::<usize>()?;
			buffer.clear();

			// MaximumColorValue
			reader.read_line(&mut buffer)?;
			let maximum_color_value = buffer.trim().parse::<Color>()?;

			let mut image = Image::new(width, height, maximum_color_value);

			let mut value_buffer = [0; 3];
			for i in 0..image.total_pixels {
				reader.read_exact(&mut value_buffer)?;
				*image.pixels.index_mut(i) = value_buffer;
			}

			Ok(image)
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

		pub fn get(&'a self, x: isize, y: isize) -> Option<&'a Pixel> {
			if x < 0 || y < 0 {
				None
			} else {
				let ux = x as usize;
				let uy = y as usize;
				let i = self.combine_index(ux, uy);
				self.pixels.get(i)
			}
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

	fn boxed_error(message: &str) -> Result<Image, Box<dyn std::error::Error>> {
		Err(Box::new(std::io::Error::new(ErrorKind::Other, message)))
	}
}