pub mod rectangle {
	#[derive(Copy, Clone)]
	pub struct Rectangle {
		pub x: usize,
		pub y: usize,
		pub width: usize,
		pub height: usize,
		pub end_x: usize,
		pub end_y: usize,
	}

	impl Rectangle {
		pub fn new(x: usize, y: usize, width: usize, height: usize) -> Rectangle {
			Rectangle {
				x,
				y,
				width,
				height,
				end_x: x + width,
				end_y: y + height,
			}
		}
	}
}