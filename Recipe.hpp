#ifndef RECIPE_HPP
#define RECIPE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Inventory.hpp"

class Recipe {
	size_t width, height;
	std::vector<blockid> input;
	InventoryStack output;

public:
	
	Recipe(const char *, const char *);

};


#endif