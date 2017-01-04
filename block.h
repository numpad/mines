#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
	int id;
	rgb_t tint;
	int damage,
	    damage_max;
} block_t;

/* block defaults */
enum {
	BLOCK_GRASS, BLOCK_DIRT, BLOCK_STONE, BLOCK_TREE, BLOCK_GRASS_DECO, BLOCK_TNT,
	BLOCK_OVEN, BLOCK_OVEN_ON, BLOCK_COAL_ORE, BLOCK_IRON_ORE, BLOCK_COBBLESTONE,
	BLOCK_GRAVEL, BLOCK_WOOD, BLOCK_LEAVES, BLOCK_BRICKS, BLOCK_CHEST, BLOCK_WORKBENCH,
	BLOCK_BOOKSHELF, BLOCK_GOLD_ORE, BLOCK_DIAMOND_ORE, BLOCK_SAND, BLOCK_GLASS,
} blocktypes;

const int block_max_damages[] = {
	[BLOCK_GRASS] = 40,
	[BLOCK_DIRT] = 40,
	[BLOCK_STONE] = 140,
	[BLOCK_TREE] = 75,
	[BLOCK_GRASS_DECO] = 5,
	[BLOCK_TNT] = 5,
	[BLOCK_OVEN] = 175,
	[BLOCK_OVEN_ON] = 180,
	[BLOCK_COAL_ORE] = 170,
	[BLOCK_IRON_ORE] = 220,
	[BLOCK_COBBLESTONE] = 150,
	[BLOCK_GRAVEL] = 40,
	[BLOCK_WOOD] = 80,
	[BLOCK_LEAVES] = 10,
	[BLOCK_BRICKS] = 300,
	[BLOCK_CHEST] = 55,
	[BLOCK_WORKBENCH] = 55,
	[BLOCK_BOOKSHELF] = 40,
	[BLOCK_GOLD_ORE] = 160,
	[BLOCK_DIAMOND_ORE] = 310,
	[BLOCK_SAND] = 40,
	[BLOCK_GLASS] = 15,
};
const int block_does_collide[] = {
	[BLOCK_GRASS] = 1,
	[BLOCK_DIRT] = 1,
	[BLOCK_STONE] = 1,
	[BLOCK_TREE] = 1,
	[BLOCK_GRASS_DECO] = 0,
	[BLOCK_TNT] = 1,
	[BLOCK_OVEN] = 1,
	[BLOCK_OVEN_ON] = 1,
	[BLOCK_COAL_ORE] = 1,
	[BLOCK_IRON_ORE] = 1,
	[BLOCK_COBBLESTONE] = 1,
	[BLOCK_GRAVEL] = 1,
	[BLOCK_WOOD] = 1,
	[BLOCK_LEAVES] = 1,
	[BLOCK_BRICKS] = 1,
	[BLOCK_CHEST] = 1,
	[BLOCK_WORKBENCH] = 1,
	[BLOCK_BOOKSHELF] = 1,
	[BLOCK_GOLD_ORE] = 1,
	[BLOCK_DIAMOND_ORE] = 1,
	[BLOCK_SAND] = 1,
	[BLOCK_GLASS] = 1,
};

block_t block_new(int id) {
	block_t block;
	block.id = id;
	block.tint = rgb_new(255, 255, 255);
	block.damage = 0;
	block.damage_max = block_max_damages[id];
	if (block.damage_max == 0) {
		block.damage_max = 50;
	}
	return block;
}

int block_collides(block_t block) {
	if (block.id < 0)
		return 0;
	return block_does_collide[block.id];
}


void block_draw(qw_image spr, int x, int y, block_t block, float xoff, float yoff, const int block_size) {
	if (block.id < 0)
		return;
	qw_image_colormod(spr, block.tint.r, block.tint.g, block.tint.b);
	qw_drawimage_quad(spr, (int)(xoff + (float)x * (float)block_size), (int)(yoff + (float)y * (float)block_size), block_size, block_size, (int)(block.id % 10) * 32, (int)(block.id / 10) * 32, 32, 32);
	float damage_percent = (float)block.damage / (float)block.damage_max;
	if (damage_percent > 0) {
		int damage_id = 9 * 10 + damage_percent * 10;
		qw_drawimage_quad(spr, (int)(xoff + (float)x * (float)block_size), (int)(yoff + (float)y * (float)block_size), block_size, block_size, (int)(damage_id % 10) * 32, (int)(damage_id / 10) * 32, 32, 32);
	}
	qw_image_nocolormod(spr);
}
#endif
