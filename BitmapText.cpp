#include "BitmapText.hpp"

BitmapText::BitmapText(BitmapFont &refFont) : font{refFont} {
	BitmapText::backgroundColor = sf::Color::Transparent;
}

void BitmapText::write(std::string text) {
	Vec2 textSize = BitmapText::font.getDimensions(text);

	if (!BitmapText::renderText.create(textSize.x, textSize.y)) {
		printf("[BitmapText] Failed creating %dx%d RenderTexture!\n", (int)textSize.x, (int)textSize.y);
		return;
	}

	BitmapText::renderText.clear(BitmapText::backgroundColor);
	for (size_t i = 0; i < text.size(); ++i) {
		BitmapText::font.writeLetter(BitmapText::renderText, Vec2((float)i * BitmapText::font.getLetterSize().width + (float)i * BitmapText::font.getSpacing().x, 0), text.at(i));
	}
	BitmapText::renderText.display();

	/* Create Sprite */
	BitmapText::renderSprite.setTexture(BitmapText::renderText.getTexture());
}

void BitmapText::drawTo(sf::RenderWindow &window, Vec2 pos) {
	BitmapText::renderSprite.setPosition(pos.x, pos.y);
	window.draw(BitmapText::renderSprite);
}