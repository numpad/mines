#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "rgb.hpp"
#include "Vec2.hpp"
#include "ParticleConfig.hpp"
#include "Grid.hpp"
#include <functional>

class Particle {
	unsigned int lifetime,
	             ticks;
	
	sf::RectangleShape shape;

	bool hasDeathfunc;
	std::function<void(Particle*)> deathfunc;
	bool hasUpdatefunc;
	std::function<void(Particle*)> updatefunc;
public:
	Vec2 pos, vel;
	
	Vec2 size;

	bool collides_blocks;
	bool dead;

	RGB color;

	Particle(Vec2 position, Vec2 velocity, ParticleConfig config);
	~Particle();

	bool dead_by_age();

	void update(Grid& grid);
	void render(sf::RenderWindow& window, Grid& grid);

	void registerDeath(std::function<void(Particle*)> deathfunc);
	void registerUpdate(std::function<void(Particle*)> updatefunc);
};

#endif