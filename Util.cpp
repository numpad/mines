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

void Util::limit(int& val, int max) {
	if (val > max)
		val = max;
}

bool Util::file_exists(const char *fn) {
	FILE *fp = fopen(fn, "r");
	if (!fp)
		return false;
	
	fclose(fp);
	return true;
}