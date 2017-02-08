#ifndef INVENTORYGUI_HPP
#define INVENTORYGUI_HPP

#include "Vec2.hpp"
#include "Block.hpp"
#include "Inventory.hpp"
#include "BitmapFont.hpp"
#include "Input.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class InventoryGUI {
	
	std::vector<Vec2> itemCells;
	Inventory items;
	InventoryStack selectedItemStack;

public:

	InventoryGUI(size_t itemCount = 40);

	void addCell(Vec2);
	Vec2 &getCell(size_t);
	bool intersectsCell(size_t, Vec2, Vec2);

	size_t getSize();

	Inventory &getItems();
	InventoryStack &getSelectedItem();

	void render(sf::RenderWindow &, Vec2, BitmapFont &);
};

#endif