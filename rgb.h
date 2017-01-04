#ifndef RGB_H
#define RGB_H

typedef struct {
	unsigned char r, g, b;
} rgb_t;

rgb_t rgb_new(unsigned char r, unsigned char g, unsigned char b) {
	rgb_t rgb;
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	return rgb;
}

/* Compares an rgb_t with 3 rgb values */
int rgb_cmp(rgb_t rgb, unsigned char r, unsigned char g, unsigned char b) {
	return (rgb.r == r && rgb.g == g && rgb.b == b);
}

/* Adds two colors */
rgb_t rgb_add(rgb_t a, rgb_t b) {
	return rgb_new(a.r + b.r, a.g + b.g, a.b + b.b);
}

#endif
