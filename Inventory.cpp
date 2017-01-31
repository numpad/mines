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
		InventoryStack::count = InventoryStack::size;
		return elements - InventoryStack::spaceLeft();
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
	return BLOCK_AIR;
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

InventoryStack &Inventory::at(size_t idx) {
	return content.at(idx);
}

bool Inventory::add(InventoryStack elems) {
	for (size_t i = 0; i < Inventory::getSize(); ++i) {
		if (Inventory::at(i).isFree()) {
			Inventory::at(i) = elems;
			return true;
		}
	}
	return false;
}