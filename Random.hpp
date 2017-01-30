#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Random {
	std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

public:
	float min, max;

	Random(float, float);
	Random();
	Random(Random&&);
	
	Random& operator=(Random&&);

	void setRange(float, float);

	float operator()();
};

#endif