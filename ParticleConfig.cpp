#include "ParticleConfig.hpp"

ParticleConfig::ParticleConfig(int lifetime_min, int lifetime_max, int size_min, int size_max) {
	ParticleConfig::colors = std::vector<RGB>();
	ParticleConfig::lifetime_rng = RNG(lifetime_min, lifetime_max);
	ParticleConfig::size_rng = RNG(size_min, size_max);

}

ParticleConfig::~ParticleConfig() {

}

void ParticleConfig::add_color(RGB color) {
	ParticleConfig::colors.push_back(color);
}

RGB ParticleConfig::get_color() {
	return ParticleConfig::colors.at(rand() % ParticleConfig::colors.size());
}

int ParticleConfig::get_lifetime() {
	return ParticleConfig::lifetime_rng.next();
}

int ParticleConfig::get_size() {
	return ParticleConfig::size_rng.next();
}
