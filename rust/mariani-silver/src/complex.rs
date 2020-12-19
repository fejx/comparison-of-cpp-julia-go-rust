pub mod complex {
	use std::ops::{Add, AddAssign, Mul};

	#[derive(Copy, Clone)]
	pub struct Complex {
		pub imaginary: f64,
		pub real: f64,
	}

	impl Complex {
		pub fn new(real: f64, imaginary: f64) -> Complex {
			Complex {
				real,
				imaginary,
			}
		}

		pub fn square(&mut self) {
			let squared_real = self.real * self.real - self.imaginary * self.imaginary;
			self.imaginary *= self.real * 2.0;
			self.real = squared_real;
		}

		pub fn absolute_squared(self) -> f64 {
			self.real * self.real + self.imaginary + self.imaginary
		}
	}

	impl Add for Complex {
		type Output = Complex;

		fn add(self, other: Self) -> Self::Output {
			Complex {
				real: self.real + other.real,
				imaginary: self.imaginary + other.imaginary,
			}
		}
	}

	impl AddAssign for Complex {
		fn add_assign(&mut self, rhs: Self) {
			self.real += rhs.real;
			self.imaginary += rhs.imaginary;
		}
	}

	impl Mul for Complex {
		type Output = Complex;

		fn mul(self, other: Self) -> Self::Output {
			Complex {
				real: self.real * other.real - self.imaginary * other.imaginary,
				imaginary: self.real * other.imaginary + other.real * self.imaginary,
			}
		}
	}
}