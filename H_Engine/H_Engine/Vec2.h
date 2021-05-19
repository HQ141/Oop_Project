#pragma once
#include <math.h>

class Vec2
{
public:
	Vec2()
	{}
	Vec2( float x, float y )
		:
		x(x),
		y(y)
	{}

	// addition/subtraction of two vectors
	Vec2 operator+(const Vec2& rhs) const {
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2 operator-(const Vec2& rhs) const {
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2& operator+=(const Vec2& rhs) {
		return *this = *this + rhs;
	}
	Vec2& operator-=(const Vec2& rhs) {
		return *this = *this - rhs;
	}

	// multiplication/division of vector with scalar
	Vec2 operator*(float rhs) const {
		return Vec2(x * rhs, y * rhs);
	}
	Vec2 operator/(float rhs) const {
		return Vec2(x / rhs, y / rhs);
	}
	Vec2& operator*=(float rhs) {
		return *this = *this * rhs;
	}
	Vec2& operator/=(float rhs) {
		return *this = *this / rhs;
	}

	double MagnitudeSquared() const {
		return double(x) * double(x) + double(y) * double(y);
	}
	double Magnitude() const {
		return sqrt(MagnitudeSquared());
	}

	// get unit vector
	Vec2 UnitVector() const {
		const double m = Magnitude();
		if (m == 0) {
			return Vec2();
		}
		return *this / float(Magnitude());
	}
	Vec2& ConvertToUnitVector() {
		return *this = UnitVector();
	}
public:
	// this is very basic data, no need to keep it private
	float x = 0.0f;
	float y = 0.0f;
};
