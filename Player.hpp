#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Limb.hpp"
#include "Entity.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Inventory.hpp"
#include "BitmapFont.hpp"
#include "BitmapText.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class Player : public Entity {
public:
	Limb *head, *armb, *armf, *legb, *legf;

	/* Inventory */
	sf::Texture inventoryHotbarTexture;
	sf::Sprite inventoryHotbarSprite;

	enum PlaceMode {
		PLACE_BACKGROUND, PLACE_FOREGROUND
	};

	Player(Vec2 screenSize);
	
	void handleInput();

	void animate(Grid& grid);
	void update(Grid& grid);
	void render(sf::RenderWindow& window, Vec2 off = Vec2(0.0f, 0.0f));
	void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off = Vec2(0.0f, 0.0f));

	void renderInventory(sf::RenderWindow &window, Vec2 off = Vec2(0, 0));
	blockid getItem();
	blockid takeItem();
	size_t collectItems(blockid type, size_t count = 1);
	bool canCollect(blockid type);
	
	void selectItem(size_t index);

	void setPlaceMode(PlaceMode);
	bool getPlaceMode();

	bool load(const char *);
	void save(const char *);
private:
	std::vector<size_t> limb_z_index;
	
	sf::Clock timeAlive;
	float legAnimation;

	/* Inventory */
	size_t currentItemSelected;
	Inventory inventory;
	PlaceMode placeMode;

	BitmapFont textFont;

	sf::Keyboard::Key key_up, key_down, key_left, key_right, key_place_background, key_place_foreground;
};

#endif