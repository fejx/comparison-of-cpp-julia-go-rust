#include "Complex.h"

using namespace std;

ostream& operator<<(ostream& os, const Complex& complex) {
	const float imaginary_part = complex.imaginary_part;
	const bool has_imaginary_part = imaginary_part != 0;
	if (has_imaginary_part) {
		const bool is_imaginary_part_negative = imaginary_part < 0;
		return os
			<< complex.real_part
			<< (is_imaginary_part_negative ? " - " : " + ")
			<< imaginary_part << "i";
	} else
		return os << complex.real_part;
}
