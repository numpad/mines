#ifndef UTIL_H
#define UTIL_H

#include "Grid.hpp"

namespace Util {
	typedef enum {
		DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT
	} Direction;

	int raycast_down(Grid, int px, int py, int len);
	int raycast_up(Grid, int px, int py, int len);
	int raycast_left(Grid, int px, int py, int len);
	int raycast_right(Grid, int px, int py, int len);
	int raycast(Grid, int px, int py, int len, Util::Direction dir);
}

#endif