pub mod image_generator {
	use std::sync::Arc;
	use std::sync::atomic::{AtomicUsize, Ordering};
	use std::time::Duration;

	use async_std::task;

	use crate::image::image::{Image, Pixel};
	use crate::image_dimensions::image_dimensions::ImageDimensions;
	use crate::mandelbrot_pixel_calculation::mandelbrot_pixel_calculation::MandelbrotPixelCalculator;
	use crate::rectangle::rectangle::Rectangle;
	use crate::rectangle_splitter::rectangle_splitter::split_rectangle;
	use crate::wait_group::wait_group::WaitGroup;

	const MAX_RECTANGLE_SIZE: usize = 50;

	#[derive(Clone)]
	pub struct GenerationContext {
		calculator: Arc<MandelbrotPixelCalculator>,
		image: Arc<Image>,
		dimensions: Arc<ImageDimensions>,
	}

	pub fn generate(
		calculator: Arc<MandelbrotPixelCalculator>,
		image: Arc<Image>,
		dimensions: Arc<ImageDimensions>,
	) {
		assert_image_matches_dimensions(image.as_ref(), dimensions.as_ref());
		let full_rectangle = rectangle_from_dimensions(dimensions.as_ref());
		let context = GenerationContext {
			calculator,
			image,
			dimensions,
		};
		let wait_group = WaitGroup::new();
		generate_within(
			context.clone(), full_rectangle, wait_group.clone(),
		);
		wait_group.wait()
	}

	fn generate_within(
		context: GenerationContext,
		rectangle: Rectangle,
		wait_group: WaitGroup,
	) {
		if is_tiny(&rectangle) {
			generate_whole_rectangle(context, &rectangle);
		} else {
			calculate_outlines_and_split(context, &rectangle, wait_group);
		}
	}

	fn generate_whole_rectangle(
		context: GenerationContext,
		rectangle: &Rectangle,
	) {
		for x in rectangle.x..rectangle.end_x {
			for y in rectangle.y..rectangle.end_y {
				generate_point(
					context.clone(),
					x,
					y,
				);
			}
		}
	}

	fn generate_point(
		context: GenerationContext,
		x: usize,
		y: usize,
	) -> Pixel {
		const EMPTY: Pixel = [0u8, 0u8, 0u8];
		let target = context.image.index_mut_unsafe(x, y);
		if *target != EMPTY {
			return *target;
		}
		let sampled = context.dimensions.sample_to(x, y);
		let color = context.calculator.get_color_for(sampled);
		*target = color;
		*target
	}

	fn is_tiny(rectangle: &Rectangle) -> bool {
		rectangle.width <= MAX_RECTANGLE_SIZE && rectangle.height <= MAX_RECTANGLE_SIZE
	}

	fn calculate_outlines_and_split(
		context: GenerationContext,
		rectangle: &Rectangle,
		wait_group: WaitGroup,
	) {
		let reference_color_option = generate_outlines(
			context.clone(),
			rectangle,
		);

		if reference_color_option.is_some() {
			fill_whole_rectangle(
				context,
				rectangle,
				&reference_color_option.unwrap(),
			);
		} else {
			split(
				context,
				rectangle,
				wait_group,
			);
		}
	}

	fn generate_outlines(
		context: GenerationContext,
		rectangle: &Rectangle,
	) -> Option<Pixel> {
		let top_y = rectangle.y;
		let bottom_y = rectangle.end_y - 1;
		let left_x = rectangle.x;
		let right_x = rectangle.end_x - 1;

		let mut outlines_identical = true;
		let reference_color = generate_point(
			context.clone(),
			left_x,
			top_y,
		);

		for x in left_x..=right_x {
			check_outline_pixel(
				context.clone(),
				x,
				top_y,
				&reference_color,
				&mut outlines_identical,
			);
			check_outline_pixel(
				context.clone(),
				x,
				bottom_y,
				&reference_color,
				&mut outlines_identical,
			);
		}
		for y in top_y..=bottom_y {
			check_outline_pixel(
				context.clone(),
				left_x,
				y,
				&reference_color,
				&mut outlines_identical,
			);
			check_outline_pixel(
				context.clone(),
				right_x,
				y,
				&reference_color,
				&mut outlines_identical,
			);
		}
		if outlines_identical {
			Option::Some(reference_color)
		} else {
			Option::None
		}
	}

	fn check_outline_pixel(
		context: GenerationContext,
		x: usize,
		y: usize,
		reference_color: &Pixel,
		outlines_identical: &mut bool,
	) {
		let current_color = generate_point(context, x, y);
		if current_color != *reference_color {
			*outlines_identical = false;
		}
	}

	fn split(
		context: GenerationContext,
		rectangle: &Rectangle,
		wait_group: WaitGroup,
	) {
		let parts = split_rectangle(rectangle);
		for part in parts.iter() {
			let child_context = context.clone();
			let child_rect = part.clone();
			let child_wait_group = wait_group.clone();
			task::spawn(async move {
				generate_within(child_context, child_rect, child_wait_group)
			});
		}
	}

	fn fill_whole_rectangle(
		context: GenerationContext,
		rectangle: &Rectangle,
		color: &Pixel,
	) {
		for x in rectangle.x..rectangle.end_x {
			for y in rectangle.y..rectangle.end_y {
				let target = context.image.index_mut_unsafe(x, y);
				*target = *color;
			}
		}
	}

	fn assert_image_matches_dimensions(image: &Image, dimensions: &ImageDimensions) {
		assert_eq!(image.height, dimensions.height);
		assert_eq!(image.width, dimensions.width);
	}

	fn rectangle_from_dimensions(dimensions: &ImageDimensions) -> Rectangle {
		Rectangle::new(
			0, 0,
			dimensions.width, dimensions.height,
		)
	}
}