#include "Inventory.hpp"

/* InventoryStack: */

InventoryStack::InventoryStack(size_t count, blockid type, size_t maxSize) {
	InventoryStack::count = count;
	InventoryStack::size = maxSize;
	InventoryStack::type = type;
}

InventoryStack::InventoryStack() : InventoryStack(0, BLOCK_AIR, 0) {
}

bool InventoryStack::isFree() {
	return (type == BLOCK_AIR || count == 0);
}

size_t InventoryStack::spaceLeft() {
	return size - count;
}

size_t InventoryStack::add(size_t elements) {
	if (InventoryStack::spaceLeft() < elements) {
		size_t spaceLeft = InventoryStack::spaceLeft();
		InventoryStack::count = InventoryStack::size;
		return elements - spaceLeft;
	}
	InventoryStack::count += elements;
	return 0;
}

blockid InventoryStack::get() {
	if (InventoryStack::count > 0) {
		return InventoryStack::type;
	}
	return BLOCK_AIR;
}

blockid InventoryStack::take() {
	if (InventoryStack::count > 0) {
		InventoryStack::count--;
		return InventoryStack::type;
	}

	InventoryStack::type = BLOCK_AIR;
	return BLOCK_AIR;
}

void InventoryStack::render(sf::RenderTarget &target, BitmapFont &bmf, Vec2 pos) {
	Block asBlock(get());
	if (asBlock.id < 0)
		return;
	
	asBlock.render(target, pos);

	/* Blocks left */
	wchar_t itemCountStr[5];
	swprintf(itemCountStr, 4, L"%lu", InventoryStack::count);
	
	bmf.write(target, pos + Vec2(1, 1), itemCountStr, sf::Color(80, 80, 80));
	bmf.write(target, pos, itemCountStr);
}

/* Inventory: */

Inventory::Inventory(size_t elements) {
	Inventory::content = std::vector<InventoryStack>();

	for (size_t i = 0; i < elements; ++i) {
		content.push_back(InventoryStack());
	}

}

size_t Inventory::getSize() {
	return content.size();
}

bool Inventory::hasSpace() {
	for (size_t i = 0; i < Inventory::getSize(); ++i) {
		if (Inventory::at(i).isFree()) {
			return true;
		}
	}
	return false;
}

bool Inventory::hasSpaceFor(blockid type) {
	for (size_t i = 0; i < Inventory::getSize(); ++i) {
		if (Inventory::at(i).isFree() || (Inventory::at(i).type == type && Inventory::at(i).count < Inventory::at(i).size)) {
			return true;
		}
	}
	return false;
}

InventoryStack &Inventory::at(size_t idx) {
	return content.at(idx);
}

bool Inventory::add(InventoryStack elems) {
	/* Check if Inventory contains stack of same type */
	for (size_t i = 0; i < Inventory::getSize(); ++i) {
		if (Inventory::at(i).type == elems.type) {
			/* Add elements if stack type is */
			elems.count = Inventory::at(i).add(elems.count);
			
			if (elems.count == 0)
				return true;
		}
	}

	for (size_t i = 0; i < Inventory::getSize(); ++i) {
		if (Inventory::at(i).isFree()) {
			Inventory::at(i) = elems;
			return true;
		}
	}
	return false;
}
