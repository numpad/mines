#include "Clouds.hpp"

Clouds::Clouds(Vec2 screenSize) {
	Clouds::windVelocity = 0.05;
	Clouds::windOffset = 0.0;

	if (!Clouds::cloudySky.create(screenSize.x * 3, screenSize.y)) {
		puts("[Clouds] Coult not create texture!");
		return;
	}

	sf::RectangleShape cloud;
	cloud.setFillColor(sf::Color(255, 255, 255));

	Clouds::cloudySky.clear(sf::Color::Transparent);
	
	Random randomSize(130, 240);
	Random randomHeight(30, 60);
	Random randomDist(10, 100);

	float width;
	float pos = randomDist();
	while (pos < Clouds::cloudySky.getSize().x) {
		width = randomSize();
		cloud.setSize(sf::Vector2f(width, randomHeight()));
		cloud.setPosition(pos, randomHeight() + 10);

		Clouds::cloudySky.draw(cloud);

		pos += randomDist() + width;
	}
	
	Clouds::cloudySky.display();

	/* Create sprite */
	Clouds::cloudySkySprite.setTexture(Clouds::cloudySky.getTexture());
	Clouds::cloudySkySprite.setTextureRect(sf::IntRect(0, 0, screenSize.x, screenSize.y));
	Clouds::cloudySkySprite.setColor(sf::Color(255, 255, 255, 180));
}

void Clouds::update(Grid &grid) {
	Clouds::windOffset += Clouds::windVelocity;
	
	const sf::IntRect rect = Clouds::cloudySkySprite.getTextureRect();
	float nx = -grid.offset.x * 0.07 + Clouds::windOffset;
	Clouds::cloudySkySprite.setTextureRect(sf::IntRect((int)nx, 0, rect.width, rect.height));

}

void Clouds::render(sf::RenderTarget &target) {
	target.draw(Clouds::cloudySkySprite);
}