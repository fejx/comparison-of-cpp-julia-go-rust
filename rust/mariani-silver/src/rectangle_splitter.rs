pub mod rectangle_splitter {
	use crate::rectangle::rectangle::Rectangle;

	pub const PART_COUNT: usize = 2;

	#[derive(PartialEq)]
	enum Direction {
		Horizontal,
		Vertical,
	}

	struct SplittingContext<'a> {
		split_direction: Direction,
		original: &'a Rectangle,
	}

	pub fn split_rectangle(rect: &Rectangle) -> [Rectangle; PART_COUNT] {
		let split_direction = if rect.height > rect.width {
			Direction::Horizontal
		} else {
			Direction::Vertical
		};
		let context = SplittingContext {
			split_direction,
			original: rect,
		};
		return context.split();
	}

	impl<'a> SplittingContext<'_> {
		fn split(&self) -> [Rectangle; PART_COUNT] {
			let mut parts = [Rectangle::new(0, 0, 0, 0); PART_COUNT];

			let splitting_length = self.get_splitting_length();
			let split_length = splitting_length / PART_COUNT;
			let mut remainder = splitting_length % PART_COUNT;
			let mut offset = 0usize;

			for i in 0..PART_COUNT {
				let mut current_split_length = split_length;
				if remainder > 0 {
					current_split_length += 1;
					remainder -= 1;
				}
				let split_rectangle = self.split_with(current_split_length, offset);
				parts[i] = split_rectangle;

				offset += current_split_length
			}
			return parts;
		}

		fn get_splitting_length(&self) -> usize {
			match self.split_direction {
				Direction::Horizontal => self.original.height,
				Direction::Vertical => self.original.width
			}
		}

		fn split_with(&self, length: usize, offset: usize) -> Rectangle {
			match self.split_direction {
				Direction::Horizontal => Rectangle::new(
					self.original.x,
					self.original.y + offset,
					self.original.width,
					length,
				),
				Direction::Vertical => Rectangle::new(
					self.original.x + offset,
					self.original.y,
					length,
					self.original.height,
				)
			}
		}
	}
}