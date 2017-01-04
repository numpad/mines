#ifndef UTIL_H
#define UTIL_H

/* casts a ray and checks if it collides with a block */
int raycast_down(grid_t grid, int px, int py, int len) {
	for (int y = py; y < py + len; ++y) {
		int bx, by;
		grid_world_to_grid(grid, px, y, &bx, &by);
		if (block_collides(grid_getblock(grid, bx, by))) {
			return y - py;
		}
	}

	return -1;
}

/* casts a ray and checks if it collides with a block */
int raycast_up(grid_t grid, int px, int py, int len) {
	for (int y = py; y > py - len; --y) {
		int bx, by;
		grid_world_to_grid(grid, px, y, &bx, &by);
		if (block_collides(grid_getblock(grid, bx, by))) {
			return py - y;
		}
	}

	return -1;
}

/* raycast */
int raycast_right(grid_t grid, int px, int py, int len) {
	for (int x = px; x < px + len; ++x) {
		int bx, by;
		grid_world_to_grid(grid, x, py, &bx, &by);
		if (block_collides(grid_getblock(grid, bx, by))) {
			return x - px;
		}
	}

	return -1;
}

/* raycast */
int raycast_left(grid_t grid, int px, int py, int len) {
	for (int x = px; x > px - len; --x) {
		int bx, by;
		grid_world_to_grid(grid, x, py, &bx, &by);
		if (block_collides(grid_getblock(grid, bx, by))) {
			return px - x;
		}
	}

	return -1;
}

#endif
