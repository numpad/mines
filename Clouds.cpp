#include "Clouds.hpp"

Clouds::Clouds(Vec2 screenSize) {
	
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
	float pos = randomDist();
	while (pos < Clouds::cloudySky.getSize().x) {
		cloud.setSize(sf::Vector2f(randomSize(), randomHeight()));
		cloud.setPosition(pos, randomHeight() + 10);

		Clouds::cloudySky.draw(cloud);

		pos += randomDist();
	}
	
	Clouds::cloudySky.display();

	/* Create sprite */
	Clouds::cloudySkySprite.setTexture(Clouds::cloudySky.getTexture());
	Clouds::cloudySkySprite.setColor(sf::Color(255, 255, 255, 180));
}

void Clouds::update(Grid &grid) {

}

void Clouds::render(sf::RenderTarget &target) {
	target.draw(Clouds::cloudySkySprite);
}