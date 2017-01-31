#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Limb.hpp"
#include "Entity.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Inventory.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Player : public Entity {
	std::vector<size_t> limb_z_index;
	
	sf::Clock timeAlive;
	float legAnimation;

	Inventory inventory;
public:
	Limb *head, *armb, *armf, *legb, *legf;

	Player();
	
	void animate(Grid& grid);
	void update(Grid& grid);
	void render(sf::RenderWindow& window, Vec2 off = Vec2(0.0f, 0.0f));
	void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off = Vec2(0.0f, 0.0f));

	void renderInventory(sf::RenderWindow &window, Vec2 off = Vec2(0, 0));
};

#endif