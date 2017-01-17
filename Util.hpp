#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

namespace Util {

	int min(int a, int b);
	unsigned int min(unsigned int a, unsigned int b);
	unsigned int min(unsigned int a, unsigned int b, unsigned int c);

	float min(float a, float b);
	float min(float a, float b, float c);
	
	void limit(float& val, float max);
	void limit(int& n, int max);

	bool file_exists(const char *fn);
}

#endif