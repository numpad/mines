#ifndef ITEM_HPP
#define ITEM_HPP

#include "Vec2.hpp"
#include "Block.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>

class Item {
	Vec2 vel;
	blockid type;
	float deltaAngle;

	sf::Sprite sprite;
	sf::Clock timeAlive;

public:
	Vec2 pos;

	Item(Vec2, blockid, Vec2 vel = Vec2(0, 0));

	void update(Grid &);
	void render(sf::RenderWindow &, Vec2 offset = Vec2(0, 0));
};

#endif