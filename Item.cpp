#include "Item.hpp"

Item::Item(Vec2 pos, blockid type, Vec2 vel) {
	Item::pos = pos;
	Item::type = type;
	Item::vel = vel;

	Item::sprite.setTexture(*(Block::tileset));
	Item::sprite.setTextureRect(Block::getBlockSubRect(Item::type));
	Item::sprite.setOrigin(16, 16);
	Item::sprite.setScale(0.7, 0.7);

	Item::setCollectTimeout(1.0);
}

void Item::setCollectTimeout(float seconds) {
	Item::collectTimeout = seconds;
}

float Item::secondsAlive() {
	return Item::timeAlive.getElapsedTime().asSeconds();
}

bool Item::collectableBy(Player &player, float radius) {
	if (Item::secondsAlive() < Item::collectTimeout)
		return false;
	
	if (!player.canCollect(Item::type))
		return false;
	
	return (Item::pos - player.pos).length() < radius;
}

blockid Item::getType() {
	return Item::type;
}

void Item::update(Grid &grid) {
	float x_friction = 0.01;

	Item::vel.y += 0.15;

	/* Raycast left/right */
	if (grid.raycast(Item::pos + Vec2(16.0 * Item::vel.normal().x, 0.0) + grid.offset, Vec2(Item::vel.x, 0.0), fabs(Item::vel.x)) > -1.0) {
		Item::vel.x = 0;
	}

	/* Raycast up/down */
	float rayDistY = grid.raycast(Item::pos + Vec2(0, 20.0 * Item::vel.normal().y) + grid.offset, Vec2(0.0, Item::vel.y), fabs(Item::vel.y));
	if (rayDistY > -1.0) {
		Item::vel.y = 0;
		//Item::pos.y -= fabs(rayDistY);
		x_friction = 0.10;
	}

	Item::deltaAngle = Item::vel.x * (x_friction * 100.0);
	Item::vel.x *= (1.0 - x_friction);
	Item::pos += Item::vel;	
}

void Item::render(sf::RenderWindow &window, Vec2 offset) {

	Vec2 finalPos = Item::pos + offset;
	/*
	const float secondsAlive = Item::timeAlive.getElapsedTime().asSeconds();
	if (fabs(Item::vel.x) < 0.05 && fabs(Item::vel.y) < 0.05) {
		finalPos += Vec2(0, fabs(cos(secondsAlive * 1.5)) * -2.0 + fabs(sin(secondsAlive * 4.5)) * -6.0);
	}
	*/

	Item::sprite.rotate(Item::deltaAngle);
	Item::sprite.setPosition(finalPos.x, finalPos.y);
	window.draw(Item::sprite);
}