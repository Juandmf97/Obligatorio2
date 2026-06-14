#pragma once

class Color {
public :
	float r, g, b;

	Color() : r(0.0f), g(0.0f), b(0.0f) {}

	Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

	Color operator*(float k) const {
		return Color(r * k, g * k, b * k);
	}

	Color operator+(const Color& otro) const {
		return Color(r + otro.r, g + otro.g, b + otro.b);
	}
};