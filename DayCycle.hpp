#ifndef DAYCYCLE_HPP
#define DAYCYCLE_HPP

#include "rgb.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>

class DayCycle {
	unsigned int daytime, daylength;
	bool sunrise;
	unsigned int days;
	RGB color;

	/* Nightsky */
	unsigned int nightSkyId;
	float nightSkyAlpha;
	
	sf::RenderTexture nightSky;
	sf::Sprite nightSkySprite;
public:

	DayCycle(unsigned int daylength, RGB color);

	sf::Color get_color();
	float get_darkness();
	unsigned int get_day();

	bool is_night();

	void update();
	void render(sf::RenderWindow& window, Grid &grid);

	void generateNightsky(Vec2);

	void save(const char *);
	bool load(const char *);

};

#endif