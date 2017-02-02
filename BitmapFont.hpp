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

	void setLayout(std::vector<std::wstring>);
	void setDefaultLayout();

	Vec2 getDimensions(std::wstring);
	Vec2 getSpacing();
	sf::IntRect getLetterSize();

	sf::IntRect getCharRect(const wchar_t);
	void writeLetter(sf::RenderTexture &, Vec2, const wchar_t);
};

#endif