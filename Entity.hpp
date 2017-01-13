#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "rgb.hpp"
#include "rect.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Grid.hpp"
#include "Limb.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Entity {

public:
	Vec2 pos, vel, lastvel;
	Vec2 acc, maxvel;

	float jumpvel, jumpstrength;
	
	Limb body;

	sf::Texture skin;

	Entity(const char *skinpath) {
		Entity::pos = Vec2(0, 0);
		Entity::vel = Vec2(0, 0);
		Entity::lastvel = Vec2(0, 0);
		Entity::acc = Vec2(0, 0);
		Entity::maxvel = Vec2(0, 0);

		Entity::jumpvel = 0;
		Entity::jumpstrength = 0;

		if (!Entity::skin.loadFromFile(skinpath)) {
			puts("could not load entity skin");
		}
	}

	virtual void addLimb(Limb body) {
		Entity::body = body;
	}

	virtual void addChildLimb(Limb body) {
		Entity::body.addChild(body);
	}
	
	virtual void update(Grid& grid) =0;
	virtual void render(sf::RenderWindow& window, Vec2 offset = Vec2(0.0f, 0.0f)) =0;
	virtual void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 offset = Vec2(0.0f, 0.0f)) =0;
	
};

#endif