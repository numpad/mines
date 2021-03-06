#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "rgb.hpp"
#include "Vec2.hpp"
#include <SFML/Graphics.hpp>

typedef enum {
	BLOCK_AIR = -1,
	BLOCK_GRASS, BLOCK_DIRT, BLOCK_STONE, BLOCK_TREE, BLOCK_TREE_BIRCH, BLOCK_TNT,
	BLOCK_OVEN, BLOCK_OVEN_ON, BLOCK_COAL_ORE, BLOCK_IRON_ORE, BLOCK_COBBLESTONE,
	BLOCK_GRAVEL, BLOCK_WOOD, BLOCK_LEAVES, BLOCK_BRICKS, BLOCK_CHEST, BLOCK_WORKBENCH,
	BLOCK_BOOKSHELF, BLOCK_GOLD_ORE, BLOCK_DIAMOND_ORE, BLOCK_SAND, BLOCK_GLASS, BLOCK_ICE,
	BLOCK_GRASS_SNOWY, BLOCK_SNOW, BLOCK_WOOL, BLOCK_COBBLESTONE_MOSSY,
	BLOCK_GRASS_DECO_0, BLOCK_GRASS_DECO_1, BLOCK_GRASS_DECO_2, BLOCK_GRASS_DECO_3
} blockid;

const int block_max_damages[] = {
	[BLOCK_GRASS] 				=  40,
	[BLOCK_DIRT] 				=  40,
	[BLOCK_STONE] 				= 140,
	[BLOCK_TREE] 				=  75,
	[BLOCK_TREE_BIRCH] 			=  80,
	[BLOCK_TNT] 				=   5,
	[BLOCK_OVEN] 				= 175,
	[BLOCK_OVEN_ON] 			= 180,
	[BLOCK_COAL_ORE] 			= 170,
	[BLOCK_IRON_ORE] 			= 220,
	[BLOCK_COBBLESTONE] 		= 150,
	[BLOCK_GRAVEL] 				=  40,
	[BLOCK_WOOD] 				=  80,
	[BLOCK_LEAVES] 				=  10,
	[BLOCK_BRICKS] 				= 300,
	[BLOCK_CHEST] 				=  55,
	[BLOCK_WORKBENCH] 			=  55,
	[BLOCK_BOOKSHELF] 			=  40,
	[BLOCK_GOLD_ORE] 			= 160,
	[BLOCK_DIAMOND_ORE] 		= 310,
	[BLOCK_SAND] 				=  30,
	[BLOCK_GLASS] 				=  15,
	[BLOCK_ICE]					=  25,
	[BLOCK_GRASS_SNOWY]			=  44,
	[BLOCK_SNOW]				=  18,
	[BLOCK_WOOL]				=  25,
	[BLOCK_COBBLESTONE_MOSSY] 	= 155,
	[BLOCK_GRASS_DECO_0]		=   4,
	[BLOCK_GRASS_DECO_1]		=   3,
	[BLOCK_GRASS_DECO_2]		=   3,
	[BLOCK_GRASS_DECO_3]		=   3
	
};

const int block_does_collide[] = {
	[BLOCK_GRASS] 				= 1,
	[BLOCK_DIRT] 				= 1,
	[BLOCK_STONE] 				= 1,
	[BLOCK_TREE] 				= 1,
	[BLOCK_TREE_BIRCH] 			= 1,
	[BLOCK_TNT] 				= 1,
	[BLOCK_OVEN] 				= 1,
	[BLOCK_OVEN_ON] 			= 1,
	[BLOCK_COAL_ORE] 			= 1,
	[BLOCK_IRON_ORE] 			= 1,
	[BLOCK_COBBLESTONE] 		= 1,
	[BLOCK_GRAVEL] 				= 1,
	[BLOCK_WOOD] 				= 1,
	[BLOCK_LEAVES] 				= 1,
	[BLOCK_BRICKS] 				= 1,
	[BLOCK_CHEST] 				= 1,
	[BLOCK_WORKBENCH] 			= 1,
	[BLOCK_BOOKSHELF] 			= 1,
	[BLOCK_GOLD_ORE] 			= 1,
	[BLOCK_DIAMOND_ORE] 		= 1,
	[BLOCK_SAND] 				= 1,
	[BLOCK_GLASS] 				= 1,
	[BLOCK_ICE]					= 1,
	[BLOCK_GRASS_SNOWY]			= 1,
	[BLOCK_SNOW]				= 1,
	[BLOCK_WOOL]				= 1,
	[BLOCK_COBBLESTONE_MOSSY] 	= 1,
	[BLOCK_GRASS_DECO_0]		= 0,
	[BLOCK_GRASS_DECO_1]		= 0,
	[BLOCK_GRASS_DECO_2]		= 0,
	[BLOCK_GRASS_DECO_3]		= 0
};


const float block_friction[] = {
	[BLOCK_GRASS] 				= 0.775,
	[BLOCK_DIRT] 				= 0.775,
	[BLOCK_STONE] 				= 0.775,
	[BLOCK_TREE] 				= 0.775,
	[BLOCK_TREE_BIRCH] 			= 0.775,
	[BLOCK_TNT] 				= 0.775,
	[BLOCK_OVEN] 				= 0.775,
	[BLOCK_OVEN_ON] 			= 0.775,
	[BLOCK_COAL_ORE] 			= 0.775,
	[BLOCK_IRON_ORE] 			= 0.775,
	[BLOCK_COBBLESTONE] 		= 0.775,
	[BLOCK_GRAVEL] 				= 0.775,
	[BLOCK_WOOD] 				= 0.775,
	[BLOCK_LEAVES] 				= 0.775,
	[BLOCK_BRICKS] 				= 0.775,
	[BLOCK_CHEST] 				= 0.775,
	[BLOCK_WORKBENCH] 			= 0.775,
	[BLOCK_BOOKSHELF] 			= 0.775,
	[BLOCK_GOLD_ORE] 			= 0.775,
	[BLOCK_DIAMOND_ORE] 		= 0.775,
	[BLOCK_SAND] 				= 0.775,
	[BLOCK_GLASS] 				= 0.775,
	[BLOCK_ICE] 				= 0.975,
	[BLOCK_GRASS_SNOWY]			= 0.775,
	[BLOCK_SNOW]				= 0.765,
	[BLOCK_WOOL]				= 0.775,
	[BLOCK_COBBLESTONE_MOSSY] 	= 0.775,
	[BLOCK_GRASS_DECO_0]		= 0.000,
	[BLOCK_GRASS_DECO_1]		= 0.000,
	[BLOCK_GRASS_DECO_2]		= 0.000,
	[BLOCK_GRASS_DECO_3]		= 0.000
	
};

class Block {
	RGB tint;
	int damage_max;

public:
	static sf::Texture *tileset;
	sf::Sprite sprite;

	blockid id;
	int damage, last_damage;

	float friction;


	Block(int);
	
	void updateId(int newid);
	
	bool collides();
	void render(sf::RenderTarget& window, float x, float y, float xoff, float yoff);
	void render(sf::RenderTarget& window, Vec2 pos, Vec2 offset = Vec2(0, 0));
	int maxDamage();

	static void setGlobalTileset(sf::Texture& texture);
	static sf::IntRect getBlockSubRect(blockid b_id);

	void applyDamage();
	void setTint(RGB nt);
	RGB& getTint();
};

#endif