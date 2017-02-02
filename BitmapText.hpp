#ifndef BITMAPTEXT_HPP
#define BITMAPTEXT_HPP

#include "Vec2.hpp"
#include "BitmapFont.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class BitmapText {
	sf::RenderTexture renderText;
	sf::Sprite renderSprite;

	sf::Color backgroundColor;

	BitmapFont &font;
public:

	BitmapText(BitmapFont &);

	void write(std::string);

	void drawTo(sf::RenderWindow &, Vec2);
};

#endif