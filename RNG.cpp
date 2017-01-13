#include "RNG.hpp"

RNG::RNG() {
	RNG::min = 0;
	RNG::max = 100;
}

RNG::RNG(int min, int max) {
	RNG::min = min;
	RNG::max = max;
}

int RNG::next() {
	return (rand() % (RNG::max - RNG::min)) + RNG::min;
}