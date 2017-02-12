#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include "Grid.hpp"
#include <SFML/Graphics.hpp>

class Minimap {

	sf::Texture texture;
	sf::Sprite sprite;

	sf::Uint8 *pixels;
public:
	int width, height;

	Minimap(int, int);
	~Minimap();
	
	/* Sets minimap pixel at index i to color c */
	void set(size_t, sf::Color color = sf::Color(22, 22, 22));
	void set(size_t, size_t, sf::Color color = sf::Color(22, 22, 22));
	void updateTexture();

	void update(Grid &, int, int);

	void draw(sf::RenderTarget &);
};

#endif