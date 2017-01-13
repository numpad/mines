#ifndef RGB_HPP
#define RGB_HPP

#include <SFML/Graphics.hpp>

class RGB {
public:
	unsigned char r, g, b, a;

	RGB(unsigned char rr = 255, unsigned char gg = 255, unsigned char bb = 255, unsigned char aa = 255);

	sf::Color toColor();

	bool operator==(RGB other);
	RGB operator+(const RGB other);
	void operator+=(const RGB other);
};

#endif