#ifndef BITMAPTEXT_HPP
#define BITMAPTEXT_HPP

#include "Vec2.hpp"
#include "BitmapFont.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class BitmapText {
	sf::RenderTexture renderText;
	sf::Sprite renderSprite;

	sf::Color textColor, backgroundColor;

	BitmapFont &font;
public:

	BitmapText(BitmapFont &);

	/* Returns dimensions of the text */
	Vec2 getSize();

	void setBackgroundColor(sf::Color);
	void setColor(sf::Color);

	/* Writes text */
	void write(std::wstring);

	/* Draws text to screen */
	void drawTo(sf::RenderWindow &, Vec2);
};

#endif