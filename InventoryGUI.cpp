#include "InventoryGUI.hpp"

InventoryGUI::InventoryGUI(size_t itemCount) : items(itemCount) {
	InventoryGUI::itemCells = std::vector<Vec2>();

}

void InventoryGUI::addCell(Vec2 pos) {
	InventoryGUI::itemCells.push_back(pos);
}

Vec2 &InventoryGUI::getCell(size_t i) {
	return InventoryGUI::itemCells.at(i);
}

bool InventoryGUI::intersectsCell(size_t i, Vec2 pos, Vec2 offset) {
	const Vec2 itemPos = InventoryGUI::getCell(i) + offset;
	if (pos.x >= itemPos.x && pos.y >= itemPos.y) {
		if (pos.x <= itemPos.x + 32.0 && pos.y <= itemPos.y + 32.0) {
			return true;
		}
	}

	return false;
}

size_t InventoryGUI::getSize() {
	return InventoryGUI::itemCells.size();
}

Inventory &InventoryGUI::getItems() {
	return InventoryGUI::items;
}

void InventoryGUI::render(sf::RenderWindow &target, Vec2 guiOffset, BitmapFont &font) {
	Vec2 mousePos(sf::Mouse::getPosition(target).x, sf::Mouse::getPosition(target).y);

	for (size_t i = 0; i < InventoryGUI::itemCells.size(); ++i) {
		Block invblock(InventoryGUI::items.at(i).get());
		
		/* Try selecting an item */
		if (Input::isMouseClicked(sf::Mouse::Left) && InventoryGUI::intersectsCell(i, mousePos, guiOffset)) {
			InventoryStack lastSelected = InventoryGUI::selectedItemStack;
			InventoryGUI::selectedItemStack = InventoryGUI::items.at(i);
			InventoryGUI::items.at(i) = lastSelected;
		}
		
		if (invblock.id < 0)
			continue;
		
		Vec2 finalPos = InventoryGUI::itemCells.at(i) + guiOffset;
		items.at(i).render(target, font, finalPos);
	}

	if (!InventoryGUI::selectedItemStack.isFree()) {
		InventoryGUI::selectedItemStack.render(target, font, mousePos);
	}
}