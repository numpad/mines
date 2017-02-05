#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Block.hpp"
#include "Vec2.hpp"
#include "BitmapFont.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class InventoryStack {
public:
	size_t count, size;
	blockid type;
	
	InventoryStack(size_t count, blockid type, size_t maxSize = 64);
	InventoryStack();

	/* Returns if this stack is free (no elements or blockid == AIR) */
	bool isFree();

	/* Returns how many elements can be added to the stack before it's full */
	size_t spaceLeft();

	/* Add n elements to the stack, returns count of elements that could not be added because the stack is too small */
	size_t add(size_t elements = 1);
	/* Return current blocktype, if no elements are left return BLOCK_AIR */
	blockid get();
	/* Acts like InventoryStack::get() but shrinks the stack by one every call */
	blockid take();

	void render(sf::RenderTarget &, BitmapFont &, Vec2);
};

class Inventory {

	std::vector<InventoryStack> content;

public:

	Inventory(size_t);

	/* Returns max. number of available stacks */
	size_t getSize();
	
	/* Returns true if at least one InventoryStack does not hold any items */
	bool hasSpace();
	bool hasSpaceFor(blockid);

	/* Returns the InventoryStack at index */
	InventoryStack &at(size_t);

	/* Adds InventoryStack to next free space, returns false if no free InventoryStack was found */
	bool add(InventoryStack);
};

#endif