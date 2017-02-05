#include "BitmapFont.hpp"

BitmapFont::BitmapFont(const char *path, Vec2 charSize, float scale, Vec2 spacing) {
	if (!BitmapFont::bitmap.loadFromFile(path)) {
		printf("[BitmapFont] Could not load file \"%s\".\n", path);
	}

	BitmapFont::letterRect = sf::IntRect(0, 0, (int)charSize.x, (int)charSize.y);
	BitmapFont::letterSpacing = spacing;
	BitmapFont::scale = scale;

	BitmapFont::letterLayout = std::vector<std::wstring>();

	BitmapFont::charSprite.setTexture(BitmapFont::bitmap);
	BitmapFont::charSprite.setScale(BitmapFont::scale, BitmapFont::scale);

	BitmapFont::setDefaultLayout();
}

void BitmapFont::setLayout(std::vector<std::wstring> layout) {
	BitmapFont::letterLayout = layout;
}

void BitmapFont::setDefaultLayout() {
	std::vector<std::wstring> layout = std::vector<std::wstring>();

	layout.push_back(L"abcdefghijklmnopqrstuvwxyz");
	layout.push_back(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	layout.push_back(L"0123456789+-*/!\"$%&()=?.:,");
	layout.push_back(L";[]{}'^<>|_#~€@äöüÄÖÜß\\°  ");

	BitmapFont::setLayout(layout);
}

sf::IntRect BitmapFont::getCharRect(const wchar_t c) {
	for (size_t y = 0; y < BitmapFont::letterLayout.size(); ++y) {
		for (size_t x = 0; x < BitmapFont::letterLayout.at(y).size(); ++x) {
			if (c == BitmapFont::letterLayout.at(y).at(x)) {
				return sf::IntRect(
					x * BitmapFont::letterRect.width, y * BitmapFont::letterRect.height,
					BitmapFont::letterRect.width, BitmapFont::letterRect.height
				);
			}
		}
	}

	/* return bottom-right character if no other was found */
	size_t yLast = BitmapFont::letterLayout.size() - 1;
	size_t xLast = BitmapFont::letterLayout.at(yLast).size() - 1;
	return sf::IntRect(
		BitmapFont::letterRect.width * xLast, BitmapFont::letterRect.height * yLast,
		BitmapFont::letterRect.width, BitmapFont::letterRect.height
	);
}

Vec2 BitmapFont::getDimensions(std::wstring text) {
	if (text.empty())
		return Vec2(0, BitmapFont::letterRect.height * BitmapFont::scale);
	
	/* Calculate the longest line */
	size_t longestLine = 0, currentLine = 0, linesCount = 0;
	for (size_t i = 0; i < text.size(); ++i) {
		if (text.at(i) == L'\n' || i == text.size() - 1) {
			++linesCount;
			
			if (!(text.at(i) == L'\n'))
				++currentLine;

			if (currentLine > longestLine)
				longestLine = currentLine;
			
			currentLine = 0;
			continue;
		} else {
			++currentLine;
		}
	}

	return Vec2(
		(float)longestLine * (BitmapFont::letterRect.width * BitmapFont::scale) + (float)(longestLine) * (BitmapFont::letterSpacing.x * BitmapFont::scale),
		(float)linesCount * (BitmapFont::letterRect.height * BitmapFont::scale) + (float)(linesCount) * (BitmapFont::letterSpacing.y * BitmapFont::scale)
	);
}

Vec2 BitmapFont::getSpacing() {
	return BitmapFont::letterSpacing * BitmapFont::scale;
}

sf::IntRect BitmapFont::getLetterSize() {
	return sf::IntRect(
		0, 0,
		BitmapFont::letterRect.width * BitmapFont::scale, BitmapFont::letterRect.height * BitmapFont::scale
	);
}

float BitmapFont::getScale() {
	return BitmapFont::scale;
}

void BitmapFont::setScale(float scale) {
	BitmapFont::scale = scale;
	BitmapFont::charSprite.setScale(scale, scale);
}

void BitmapFont::writeLetter(sf::RenderTarget &rtex, Vec2 pos, const wchar_t c, sf::Color fgColor) {
	/* do not draw space */
	if (c == ' ')
		return;
	
	const sf::IntRect charRect = BitmapFont::getCharRect(c);
	BitmapFont::charSprite.setTextureRect(charRect);
	charSprite.setPosition(pos.x, pos.y);
	charSprite.setColor(fgColor);

	rtex.draw(charSprite);
}

void BitmapFont::write(sf::RenderTarget &rtex, Vec2 pos, std::wstring text, sf::Color textColor) {
	size_t line_x = 0,
	       line_y = 0;
	for (size_t i = 0; i < text.size(); ++i) {
		if (text.at(i) == L'\n') {
			line_x = 0;
			++line_y;
			continue;
		}
		BitmapFont::writeLetter(rtex,
								Vec2(
									pos.x + ((float)line_x * BitmapFont::getLetterSize().width  + (float)line_x * BitmapFont::getSpacing().x),
									pos.y + ((float)line_y * BitmapFont::getLetterSize().height + (float)line_y * BitmapFont::getSpacing().y)
									),
								text.at(i), textColor);
		
		++line_x;
	}
}