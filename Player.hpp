#ifndef PLAYER_HPP
#define PLAYER_HPP

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#include "Limb.hpp"
#include "Entity.hpp"
#include "Vec2.hpp"
#include "Block.hpp"
#include "Inventory.hpp"
#include "BitmapFont.hpp"
#include "BitmapText.hpp"
#include "InventoryGUI.hpp"
#include "Input.hpp"
#include "Item.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class Player : public Entity {
public:
	Limb *head, *armb, *armf, *legb, *legf;
	bool showInventory;

	/* Inventory */
	sf::Texture inventoryHotbarTexture, inventoryFullTexture;
	sf::Sprite inventoryHotbarSprite, inventoryFullSprite;

	enum PlaceMode {
		PLACE_BACKGROUND, PLACE_FOREGROUND
	};

	Player(Vec2 screenSize);
	
	void handleInput();

	void animate(Grid& grid);
	void update(Grid& grid);
	void render(sf::RenderWindow& window, Vec2 off = Vec2(0.0f, 0.0f));
	void render(sf::RenderWindow& window, sf::Shader& shader, Vec2 off = Vec2(0.0f, 0.0f));

	void renderInventory(sf::RenderWindow &window, std::vector<Item> &droppedItems, Vec2 off = Vec2(0, 0));
	blockid getItem();
	blockid takeItem();
	size_t collectItems(blockid type, size_t count = 1);
	bool canCollect(blockid type);
	
	bool canCollectItem(Item &item, float radius = 64.0);

	void selectItem(size_t index);

	void animateArm(float angle, float time = 5.0);

	void setPlaceMode(PlaceMode);
	bool getPlaceMode();

	bool load(const char *);
	void save(const char *);
private:
	std::vector<size_t> limb_z_index;
	
	sf::Clock timeAlive;
	float legAnimation;

	float armfAngle, armfAnimation;

	/* Inventory */
	size_t currentItemSelected;
	InventoryGUI inventoryGui;


	PlaceMode placeMode;

	BitmapFont textFont;

	sf::Keyboard::Key key_up, key_down, key_left, key_right,
	                  key_place_background, key_place_foreground,
					  key_inventory;
};

#endif