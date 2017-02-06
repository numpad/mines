#ifndef INVENTORYGUI_HPP
#define INVENTORYGUI_HPP

#include "Vec2.hpp"
#include "Block.hpp"
#include "Inventory.hpp"
#include "BitmapFont.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class InventoryCell {
	
public:
	Vec2 pos;
	InventoryStack stack;

	InventoryCell(Vec2, InventoryStack stack = InventoryStack());
};

class InventoryGUI {
	
	std::vector<InventoryCell> itemCells;

public:

	InventoryGUI();

	void addCell(Vec2);
	InventoryCell &getCell(size_t);

	void render(sf::RenderTarget &, Vec2, BitmapFont &);
};

#endif