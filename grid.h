#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include "rgb.h"
#include "block.h"

typedef struct {
	/* block properties */
	int width, height;
	int blocksize;
	/* camera */
	float xoff, yoff;
	/* map */
	block_t *blocks;

	/* tileset */
	qw_image spritesheet;
} grid_t;

grid_t grid_new(int w, int h, int bs, const char *tileset_path) {
	grid_t grid;
	grid.width = w;
	grid.height = h;
	grid.xoff = 0.0;
	grid.yoff = 0.0;
	grid.blocksize = bs;
	grid.blocks = malloc(w * h * sizeof(block_t));
	
	for (int i = 0; i < w * h; ++i) {
		grid.blocks[i] = block_new(-1);
	}
	
	grid.spritesheet = qw_loadimage(tileset_path);

	return grid;
}

void grid_delete(grid_t *grid) {
	free(grid->blocks);
	qw_destroyimage(grid->spritesheet);
}

/* save current world to file */
void grid_saveto(grid_t *grid, const char *filename) {
	FILE *fp = fopen(filename, "w+");
	if (!fp) {
		printf("could not write to \"%s\"!\n", filename);
		return;
	}
	
	fwrite(&grid->width, sizeof(int), 1, fp);
	fwrite(&grid->height, sizeof(int), 1, fp);
	fwrite(grid->blocks, sizeof(block_t), grid->width * grid->height, fp);

	fclose(fp);
}

/* load world from file, if not exists create a new one */
grid_t grid_loadfrom(const char *filename) {
	FILE *fp = fopen(filename, "r+");
	if (!fp) {
		printf("coult not load from \"%s\"!\n", filename);

		grid_t newgrid = grid_new(120, 50, -1, "assets/spritesheet.png");
		return newgrid;
	}
	
	int loaded_dimensions[2] = {0};
	fread(loaded_dimensions, sizeof(int), 2, fp);
	block_t loaded_blocks[loaded_dimensions[0] * loaded_dimensions[1]];
	fread(loaded_blocks, sizeof(block_t), loaded_dimensions[0] * loaded_dimensions[1], fp);

	grid_t loadedgrid = grid_new(loaded_dimensions[0], loaded_dimensions[1], 32, "assets/spritesheet.png");
	for (int i = 0; i < loaded_dimensions[0] * loaded_dimensions[1]; ++i) {
		loadedgrid.blocks[i] = loaded_blocks[i];
	}

	fclose(fp);
	return loadedgrid;
}

block_t grid_getblock(grid_t grid, int x, int y) {
	if (x < 0 || y < 0 || x >= grid.width || y >= grid.height)
		return block_new(-1);
	
	return grid.blocks[x + y * grid.width];
}

void grid_setblock(grid_t grid, int x, int y, block_t block) {
	if (x < 0 || y < 0 || x >= grid.width || y >= grid.height)
		return;
	
	grid.blocks[x + y * grid.width] = block;
}

block_t *grid_at(grid_t grid, int x, int y) {
	if (x < 0 || y < 0 || x >= grid.width || y >= grid.height)
		return NULL;
	
	return &grid.blocks[x + y * grid.width];
}

rgb_t grid_gettint(grid_t grid, int x, int y) {
	if (x < 0 || y < 0 || x >= grid.width || y >= grid.height)
		return rgb_new(255, 255, 255);
	
	return grid.blocks[x + y * grid.width].tint;
}

void grid_settint(grid_t grid, int x, int y, rgb_t tint) {
	if (x < 0 || y < 0 || x >= grid.width || y >= grid.height)
		return;
	
	grid.blocks[x + y * grid.width].tint = tint;
}

int grid_neighbors(grid_t grid, int xp, int yp) {
	int sum = 0;
	for (int y = yp - 1; y <= yp + 1; ++y) {
		for (int x = xp - 1; x <= xp + 1; ++x) {
			if (x == xp && y == yp)
				continue;

			if (x < 0 || y < 0 || x >= grid.width || y >= grid.height) {
				continue;
			}
			
			if (grid_getblock(grid, x, y).id >= 0) {
				++sum;
			}
		}
	}
	
	return sum;
}

int grid_isfree(grid_t grid, int xp, int yp, int w, int h) {
	for (int y = yp; y < yp + h; ++y) {
		for (int x = xp; x < xp + w; ++x) {
			if (grid_getblock(grid, x, y).id >= 0)
				return 0;
		}
	}

	return 1;
}

void grid_world_to_grid(grid_t grid, int worldx, int worldy, int *x, int *y) {
	*x = (worldx - grid.xoff) / grid.blocksize;
	*y = (worldy - grid.yoff) / grid.blocksize;
}


void grid_draw(grid_t grid, int xs, int ys, int w, int h) {
	for (int y = ys; y < ys + h; ++y) {
		for (int x = xs; x < xs + w; ++x) {
			if (x < 0 || x >= grid.width || y < 0 || y >= grid.height)
				continue;

			block_draw(grid.spritesheet, x, y, grid_getblock(grid, x, y), grid.xoff, grid.yoff, grid.blocksize);
		}
	}
}
#endif
