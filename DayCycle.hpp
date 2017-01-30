#ifndef DAYCYCLE_HPP
#define DAYCYCLE_HPP

#include "rgb.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>

class DayCycle {
	unsigned int daytime, daylength;
	bool sunrise;
	unsigned int days;
	RGB color;
public:

	DayCycle(unsigned int daylength, RGB color);

	sf::Color get_color();
	float get_darkness();
	unsigned int get_day();

	void update();
	void render(sf::RenderWindow& window, Grid &grid);
};

#endif