#ifndef MARIANI_SILVER_COMPLEX_H
#define MARIANI_SILVER_COMPLEX_H

#include <ostream>

class Complex {
public:
	Complex(float real_part = 0, float imaginary_part = 0) :
		real_part{real_part}, imaginary_part{imaginary_part} {}

	friend std::ostream& operator<<(std::ostream& os, const Complex& complex);

	[[nodiscard]] float getRealPart() const {
		return real_part;
	}

	[[nodiscard]] float getImaginaryPart() const {
		return imaginary_part;
	}

	[[nodiscard]] Complex operator+(const Complex& other) const {
		const float summed_real_part = real_part + other.real_part;
		const float summed_imaginary_part = imaginary_part + other.imaginary_part;
		return Complex(summed_real_part, summed_imaginary_part);
	}

	Complex& operator+=(const Complex& other) {
		real_part += other.real_part;
		imaginary_part += other.imaginary_part;
		return *this;
	}

	[[nodiscard]] Complex operator*(const Complex& other) const {
		const float multiplied_real_part = real_part * other.real_part - imaginary_part * other.imaginary_part;
		const float multiplied_imaginary_part = real_part * other.imaginary_part + other.real_part * imaginary_part;
		return Complex(multiplied_real_part, multiplied_imaginary_part);
	}

	[[nodiscard]] float absolute_squared() const {
		return real_part * real_part + imaginary_part * imaginary_part;
	}

	void square() {
		auto const squared_real_part = real_part * real_part - imaginary_part * imaginary_part;

		imaginary_part *= real_part * 2;
		real_part = squared_real_part;
	}

	void move_by(const float real_part_offset, const float imaginary_part_offset) {
		real_part += real_part_offset;
		imaginary_part += imaginary_part_offset;
	}

private:
	float real_part;
	float imaginary_part;
};

#endif //MARIANI_SILVER_COMPLEX_H
