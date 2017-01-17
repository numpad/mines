#include "Util.hpp"

int Util::min(int a, int b) {
	return a < b ? a : b;
}

unsigned int Util::min(unsigned int a, unsigned int b) {
	return a < b ? a : b;
}

unsigned int Util::min(unsigned int a, unsigned int b, unsigned int c) {
	return Util::min(Util::min(a, b), c);
}

float Util::min(float a, float b) {
	return a < b ? a : b;
}

float Util::min(float a, float b, float c) {
	return Util::min(Util::min(a, b), c);
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