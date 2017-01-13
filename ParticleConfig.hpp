#ifndef PARTICLE_CONFIG_HPP
#define PARTICLE_CONFIG_HPP

#include "RNG.hpp"
#include "rgb.hpp"
#include <vector>

class ParticleConfig {
	std::vector<RGB> colors;
	RNG lifetime_rng;
	RNG size_rng;
public:

	ParticleConfig(int, int, int, int);
	~ParticleConfig();

	void add_color(RGB color);
	RGB get_color();
	
	int get_lifetime();
	int get_size();


};

#endif