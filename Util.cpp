#include "Util.hpp"

int Util::min(int a, int b) {
	return a < b ? a : b;
}

unsigned int Util::min(unsigned int a, unsigned int b) {
	return a < b ? a : b;
}

void Util::limit(float& val, float max) {
	if (val > max)
		val = max;
}