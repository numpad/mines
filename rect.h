#ifndef RECT_H
#define RECT_H

typedef struct {
	int x, y, w, h;
} rect_t;

rect_t rect_new(int x, int y, int w, int h) {
	rect_t rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

#endif
