#ifndef CLOUDS_HPP
#define CLOUDS_HPP

#include "Vec2.hpp"
#include "Grid.hpp"
#include "Random.hpp"
#include <SFML/Graphics.hpp>

class Clouds {
	sf::RenderTexture cloudySky;
	sf::Sprite cloudySkySprite;

	float windVelocity, windOffset;
public:

	Clouds(Vec2);

	void update(Grid &);
	void render(sf::RenderTarget &);
};

#endif
