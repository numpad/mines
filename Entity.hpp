#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "rgb.hpp"
#include "rect.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Grid.hpp"
#include "Limb.hpp"
#include "Util.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Entity {

public:
	Vec2 vel, lastvel;
	Vec2 acc, maxvel;

	float jumpvel, jumpstrength;
	
	Limb body;

	sf::Texture skin;

	Entity(const char *skinpath);
	void addLimb(Limb body);
	void addChildLimb(Limb body);

	void physicsUpdate(Grid& grid);
	
	virtual void update(Grid& grid) =0;
	virtual void render(sf::RenderWindow& window, Vec2 offset = Vec2(0.0f, 0.0f)) =0;
	virtual void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 offset = Vec2(0.0f, 0.0f)) =0;
	
};

#endif