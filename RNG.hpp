#ifndef RNG_HPP
#define RNG_HPP

#include <stdlib.h>

class RNG {
	int min, max;

public:

	RNG();
	RNG(int min, int max);

	int next();
};

#endif