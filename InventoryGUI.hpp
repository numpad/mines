#ifndef INVENTORYGUI_HPP
#define INVENTORYGUI_HPP

#include "Vec2.hpp"
#include "Block.hpp"
#include "Inventory.hpp"
#include "BitmapFont.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class InventoryGUI {
	
	std::vector<Vec2> itemCells;
	Inventory items;

public:

	InventoryGUI(size_t itemCount = 40);

	void addCell(Vec2);
	Vec2 &getCell(size_t);
	bool intersectsCell(size_t, Vec2);
	
	size_t getSize();

	Inventory &getItems();

	void render(sf::RenderTarget &, Vec2, BitmapFont &);
};

#endif