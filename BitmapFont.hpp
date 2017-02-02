#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

#include "Vec2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class BitmapFont {
	sf::Texture bitmap;
	sf::IntRect letterRect;
	
	std::vector<std::wstring> letterLayout;

	float scale;
	Vec2 letterSpacing;
public:

	BitmapFont(const char *, Vec2, float scale = 1.0, Vec2 spacing = Vec2(1.0, 2.0));

	Vec2 getDimensions(std::string);
	Vec2 getSpacing();
	sf::IntRect getLetterSize();

	sf::IntRect getCharRect(const char);
	void writeLetter(sf::RenderTexture &, Vec2, const char);
};

#endif