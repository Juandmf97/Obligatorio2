#pragma once

#include <cmath>

class Color {
public :
	float r, g, b;

	Color() : r(0.0f), g(0.0f), b(0.0f) {}

	Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

	Color operator*(float k) const {
		return Color(std::fmin(1.0f, r * k), std::fmin(1.0f, g * k), std::fmin(1.0f, b * k));
	}

	Color operator+(const Color& otro) const {
		return Color(std::fmin(1.0f,r + otro.r), std::fmin(1.0f, g + otro.g), std::fmin(1.0f, b + otro.b));
	}

	Color operator*(const Color& otro) const {
		return Color(r * otro.r, g * otro.g, b * otro.b);
	}

	Color& operator+=(const Color& otro) {
		r += otro.r;
		g += otro.g; 
		b += otro.b;
		return *this;
	}

	Color operator/(int n) const {
		return Color(r / n, g / n, b / n);
	}
};
