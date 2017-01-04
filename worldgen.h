#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "block.h"
#include "grid.h"

void genworld(grid_t grid) {
	for (int y = 0; y < grid.height; ++y) {
		for (int x = 0; x < grid.width; ++x) {
			if (x == 4) {
				grid_setblock(grid, x, y, block_new(10));
			}
			
			if (y > grid.height / 3) {
				grid_setblock(grid, x, y, block_new(1));
			} else if (y == grid.height / 3) {
				grid_setblock(grid, x, y, block_new(0));
			}

		}
	}

	return;
	
	srand(time(0));
	/* initial dirt generation */
	for (int y = 0; y < grid.height; ++y) {
		for (int x = 0; x < grid.width; ++x) {
			int rnd = rand() % 100;
			int rmax = ((float)y / (float)grid.height) * 100;

			if (rnd < rmax + sin(x / 30) * 5) {
				grid_setblock(grid, x, y, block_new((rmax > (66 + (rnd - rnd/2)/2) ? 2 : 1)));
			}
		}
	}


	for (int y = 0; y < grid.height; ++y) {
		for (int x = 0; x < grid.width; ++x) {
			int neighbors = grid_neighbors(grid, x, y);
			
			if (grid_getblock(grid, x, y).id == -1 && neighbors > 4) {
				grid_setblock(grid, x, y, block_new(y > (grid.height / 1.75 + sin(x) * 2) ? 33 : 1));
			}
			
			if (grid_getblock(grid, x, y).id != -1 && neighbors <= 1) {
				grid_setblock(grid, x, y, block_new(-1));
				x -= 1;
			}	
		}
	}
	
	for (int x = 0; x < grid.width; ++x) {
		for (int y = 0; y < grid.height; ++y) {
			if (grid_getblock(grid, x, y).id == 1 && grid_getblock(grid, x, y - 1).id < 0)
				grid_setblock(grid, x, y, block_new(0));
		}
	}
	

	for (int x = 0; x < grid.width; ++x) {
		for (int y = 0; y < grid.height; ++y) {
			if (grid_getblock(grid, x, y).id == 0 && grid_getblock(grid, x, y - 1).id < 0) {
				grid_setblock(grid, x, y - 1, block_new(4));
				break;
			}
		}
	}

	for (int x = 0; x < grid.width; ++x) {
		for (int y = 0; y < grid.height; ++y) {
			
			int treesize = rand() % 4 + 5;
			if (rand() % 100 < 20 && grid_getblock(grid, x, y).id == 0 && grid_isfree(grid, x - 1, y - treesize, 3, treesize - 1)) {
				for (int ty = 1; ty < treesize; ++ty) {
					grid_setblock(grid, x, y - ty, block_new(3));
				}
				grid_setblock(grid, x - 1, y - (treesize - 1), block_new(35));
				grid_setblock(grid, x + 1, y - (treesize - 1), block_new(35));
				grid_setblock(grid, x - 1, y - treesize, block_new(35));
				grid_setblock(grid, x    , y - treesize, block_new(35));
				grid_setblock(grid, x + 1, y - treesize, block_new(35));
				grid_setblock(grid, x    , y - treesize - 1, block_new(35));
				grid_setblock(grid, x, y, block_new(1));
			}
		}
	}

}
#endif
