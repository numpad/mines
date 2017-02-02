#include "BitmapText.hpp"

BitmapText::BitmapText(BitmapFont &refFont) : font{refFont} {
	BitmapText::setBackgroundColor(sf::Color::Transparent);
	BitmapText::setColor(sf::Color::White);

}

void BitmapText::setBackgroundColor(sf::Color newcolor) {
	BitmapText::backgroundColor = newcolor;
}

void BitmapText::setColor(sf::Color textcolor) {
	BitmapText::textColor = textcolor;
}

Vec2 BitmapText::getSize() {
	return Vec2(BitmapText::renderText.getSize().x, BitmapText::renderText.getSize().y);
}

void BitmapText::write(std::wstring text) {
	Vec2 textSize = BitmapText::font.getDimensions(text);

	if (!BitmapText::renderText.create(textSize.x, textSize.y)) {
		printf("[BitmapText] Failed creating %dx%d RenderTexture!\n", (int)textSize.x, (int)textSize.y);
		return;
	}

	/* Write text to a texture */
	BitmapText::renderText.clear(BitmapText::backgroundColor);
	BitmapText::font.write(BitmapText::renderText, Vec2(0, 0), text, BitmapText::textColor);
	BitmapText::renderText.display();

	/* Create Sprite */
	BitmapText::renderSprite.setTexture(BitmapText::renderText.getTexture());
}

void BitmapText::drawTo(sf::RenderWindow &window, Vec2 pos) {
	BitmapText::renderSprite.setPosition(pos.x, pos.y);
	window.draw(BitmapText::renderSprite);
}