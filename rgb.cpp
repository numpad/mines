#include "rgb.hpp"

RGB::RGB(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
	: r{rr}, g{gg}, b{bb}, a{aa} {

}

sf::Color RGB::toColor() {
	return sf::Color(r, g, b, a);
}

bool RGB::operator==(RGB other) {
	return (r == other.r && g == other.g && b == other.b && a == other.a);
}

RGB RGB::operator+(const RGB other) {
	return RGB(r + other.r, g + other.g, b + other.b, a + other.a);
}

void RGB::operator+=(const RGB other) {
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;
}
