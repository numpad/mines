#include "Vec2.hpp"

/* Operations */

/* Adds b to a */
Vec2 Vec2::operator+(const Vec2 b) {
	return Vec2(x + b.x, y + b.y);
}

/* Adds b to a */
void Vec2::operator+=(const Vec2 b) {
	x += b.x;
	y += b.y;
}

/* Substracts b from a */
Vec2 Vec2::operator-(const Vec2 b) {
	return Vec2(x - b.x, y - b.y);
}

/* Substracts b from a */
void Vec2::operator-=(const Vec2 b) {
	x -= b.x;
	y -= b.y;
}

Vec2 Vec2::operator*(const Vec2 b) {
	return Vec2(x * b.x, y * b.y);
}

Vec2 Vec2::operator*(const float n) {
	return Vec2(x * n, y * n);
}

void Vec2::operator*=(const Vec2 b) {
	x *= b.x;
	y *= b.y;
}

void Vec2::operator*=(const float n) {
	x *= n;
	y *= n;
}

Vec2 Vec2::operator/(const Vec2 b) {
	return Vec2(x / b.x, y / b.y);
}

Vec2 Vec2::operator/(const float n) {
	return Vec2(x / n, y / n);
}

void Vec2::operator/=(const Vec2 b) {
	x /= b.x;
	y /= b.y;
}

void Vec2::operator/=(const float n) {
	x /= n;
	y /= n;
}

/* Returns length of vector */
float Vec2::length() {
	return sqrt(x * x + y * y);
}

/* Sets length of vector */
void Vec2::length(float len) {
	const Vec2 vec = normal() * len;
	x = vec.x;
	y = vec.y;
}

void Vec2::limit(float max_len) {
	if (length() > max_len) {
		length(max_len);
	}
}

/* Returns vector with length 1 */
Vec2 Vec2::normal() {
	return Vec2(x, y) / length();
}

void Vec2::normalize() {
	const float len = length();
	x /= len;
	y /= len;
}

void Vec2::rotate(float theta) {
	x = x * cos(theta) - y * sin(theta);
	y = x * sin(theta) + y * cos(theta);
}

float Vec2::angle(Vec2 b) {
	return atan2(b.y - y, b.x - x) * 180.0 / M_PI;
}

Vec2 Vec2::onlyX() {
	return Vec2(Vec2::x, 0.0);
}

Vec2 Vec2::onlyY() {
	return Vec2(0.0, Vec2::y);
}
