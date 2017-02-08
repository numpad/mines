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

InventoryStack &InventoryGUI::getSelectedItem() {
	return InventoryGUI::selectedItemStack;
}

void InventoryGUI::render(sf::RenderWindow &target, Vec2 guiOffset, BitmapFont &font) {
	Vec2 mousePos(sf::Mouse::getPosition(target).x, sf::Mouse::getPosition(target).y);

	for (size_t i = 0; i < InventoryGUI::itemCells.size(); ++i) {
		
		/* Try selecting an item */
		if (Input::isMouseClicked(sf::Mouse::Left) && InventoryGUI::intersectsCell(i, mousePos, guiOffset)) {
			/* Check if we can stack items of the same type... */
			if (InventoryGUI::items.at(i) == InventoryGUI::selectedItemStack) {
				size_t leftover = InventoryGUI::items.at(i).add(InventoryGUI::selectedItemStack.count);
				InventoryGUI::selectedItemStack = InventoryStack(leftover, InventoryGUI::items.at(i).type, InventoryGUI::items.at(i).size);
			} else { /* ...or swap them */
				InventoryStack lastSelected = InventoryGUI::selectedItemStack;
				InventoryGUI::selectedItemStack = InventoryGUI::items.at(i);
				InventoryGUI::items.at(i) = lastSelected;
			}
		}
		
		if (Input::isMouseClicked(sf::Mouse::Right) && InventoryGUI::intersectsCell(i, mousePos, guiOffset)) {
			/* Check if currently an itemstack is selected */
			if (InventoryGUI::selectedItemStack.isFree()) {
				size_t items_count = floor(InventoryGUI::items.at(i).count / 2.0);
				blockid items_type = InventoryGUI::items.at(i).type;
				size_t items_max = InventoryGUI::items.at(i).size;
				
				InventoryGUI::items.at(i).count -= items_count;
				InventoryGUI::selectedItemStack = InventoryStack(items_count, items_type, items_max);
			} else {
				blockid oneItem = InventoryGUI::selectedItemStack.get();
				if (oneItem < 0)
					continue;
				
				/* If right click with selected item(s) on free cell, place one item in it */
				if (InventoryGUI::items.at(i).isFree()) {
					InventoryGUI::selectedItemStack.take();
					InventoryGUI::items.at(i).count = 1;
					InventoryGUI::items.at(i).type = oneItem;
				} else { /* Right click on already occupied cell, check if same, space left and increase its count by one */
					if (InventoryGUI::items.at(i) == InventoryGUI::selectedItemStack && InventoryGUI::items.at(i).spaceLeft() > 0) {
						InventoryGUI::selectedItemStack.take();
						InventoryGUI::items.at(i).count += 1;
					}
				}
			}
		}
		

		if (InventoryGUI::items.at(i).get() < 0)
			continue;
		
		Vec2 finalPos = InventoryGUI::itemCells.at(i) + guiOffset;
		items.at(i).render(target, font, finalPos);
	}

	if (!InventoryGUI::selectedItemStack.isFree()) {
		InventoryGUI::selectedItemStack.render(target, font, mousePos);
	}
}