#include "BitmapText.hpp"

BitmapText::BitmapText(BitmapFont &refFont) : font{refFont} {
	BitmapText::backgroundColor = sf::Color::Transparent;
}

void BitmapText::write(std::wstring text) {
	Vec2 textSize = BitmapText::font.getDimensions(text);

	if (!BitmapText::renderText.create(textSize.x, textSize.y)) {
		printf("[BitmapText] Failed creating %dx%d RenderTexture!\n", (int)textSize.x, (int)textSize.y);
		return;
	}

	/* Write text to a texture */
	BitmapText::renderText.clear(BitmapText::backgroundColor);

	size_t line_x = 0,
	       line_y = 0;
	for (size_t i = 0; i < text.size(); ++i) {
		if (text.at(i) == L'\n') {
			line_x = 0;
			++line_y;
			continue;
		}
		BitmapText::font.writeLetter(BitmapText::renderText,
		                             Vec2(
											(float)line_x * BitmapText::font.getLetterSize().width + (float)line_x * BitmapText::font.getSpacing().x,
											(float)line_y * BitmapText::font.getLetterSize().height + (float)line_y * BitmapText::font.getSpacing().y
										 ),
									 text.at(i));
		
		++line_x;
	}
	BitmapText::renderText.display();

	/* Create Sprite */
	BitmapText::renderSprite.setTexture(BitmapText::renderText.getTexture());
}

void BitmapText::drawTo(sf::RenderWindow &window, Vec2 pos) {
	BitmapText::renderSprite.setPosition(pos.x, pos.y);
	window.draw(BitmapText::renderSprite);
}