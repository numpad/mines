#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include "Grid.hpp"
#include <SFML/Graphics.hpp>

class Minimap {

	sf::Texture texture;
	sf::Sprite sprite;

	sf::Uint8 *pixels;
public:
	size_t width, height;

	Minimap(size_t, size_t);
	~Minimap();

	/* Sets minimap pixel at index i to color c */
	void set(size_t, sf::Color color = sf::Color(22, 22, 22));
	void set(size_t, size_t, sf::Color color = sf::Color(22, 22, 22));
	void updateTexture();

	void update(Grid &, size_t, size_t);

	void draw(sf::RenderTarget &);
};

#endif