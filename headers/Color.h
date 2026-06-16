#pragma once

#include <cmath>

class Color {
public :
	float r, g, b;

	Color() : r(0.0f), g(0.0f), b(0.0f) {}

	Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

	Color operator*(float k) const {
		return Color(std::fmin(1.0, r * k), std::fmin(1.0, g * k), std::fmin(1.0, b * k));
	}

	Color operator+(const Color& otro) const {
		return Color(std::fmin(1.0,r + otro.r), std::fmin(1.0, g + otro.g), std::fmin(1.0, b + otro.b));
	}
};
