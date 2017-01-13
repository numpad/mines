#include "Util.hpp"

int Util::raycast_down(Grid grid, int px, int py, int len) {
	for (int y = py; y < py + len; ++y) {
		int bx, by;
		grid.point_to_grid(px, y, bx, by);
		if (grid.at(bx, by).collides()) {
			return y - py;
		}
	}

	return -1;
}

int Util::raycast_up(Grid grid, int px, int py, int len) {
	for (int y = py; y > py - len; --y) {
		int bx, by;
		grid.point_to_grid(px, y, bx, by);
		if (grid.at(bx, by).collides()) {
			return py - y;
		}
	}

	return -1;
}

int Util::raycast_left(Grid grid, int px, int py, int len) {
	for (int x = px; x > px - len; --x) {
		int bx, by;
		grid.point_to_grid(x, py, bx, by);
		if (grid.at(bx, by).collides()) {
			return px - x;
		}
	}

	return -1;
}

int Util::raycast_right(Grid grid, int px, int py, int len) {
	for (int x = px; x < px + len; ++x) {
		int bx, by;
		grid.point_to_grid(x, py, bx, by);
		if (grid.at(bx, by).collides()) {
			return x - px;
		}
	}

	return -1;
}

int Util::raycast(Grid grid, int px, int py, int len, Util::Direction dir) {
	switch (dir) {
		case Util::DIR_UP:
			return Util::raycast_up(grid, px, py, len);
			break;
		case Util::DIR_DOWN:
			return Util::raycast_down(grid, px, py, len);
			break;
		case Util::DIR_LEFT:
			return Util::raycast_left(grid, px, py, len);
			break;
		case Util::DIR_RIGHT:
			return Util::raycast_right(grid, px, py, len);
			break;
		
	};

	return -1;
}