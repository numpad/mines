#include "InventoryGUI.hpp"

InventoryCell::InventoryCell(Vec2 pos, InventoryStack stack) {
	InventoryCell::pos = pos;
	InventoryCell::stack = stack;
}


/* InventoryGUI */

InventoryGUI::InventoryGUI() {
	InventoryGUI::itemCells = std::vector<InventoryCell>();

}

void InventoryGUI::addCell(Vec2 pos) {
	InventoryGUI::itemCells.push_back(InventoryCell(pos));
}

InventoryCell &InventoryGUI::getCell(size_t i) {
	return InventoryGUI::itemCells.at(i);
}

void InventoryGUI::render(sf::RenderTarget &target, Vec2 guiOffset, BitmapFont &font) {
	for (size_t i = 0; i < InventoryGUI::itemCells.size(); ++i) {
		Block invblock(InventoryGUI::itemCells.at(i).stack.get());
		
		if (invblock.id < 0)
			continue;
		
		Vec2 finalPos = InventoryGUI::itemCells.at(i).pos + guiOffset;
		InventoryGUI::itemCells.at(i).stack.render(target, font, finalPos);
	}
}