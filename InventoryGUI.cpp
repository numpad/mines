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