#include "Particle.hpp"

Particle::Particle(Vec2 position, Vec2 velocity, ParticleConfig config) {
	Particle::pos = position;
	Particle::vel = velocity;
	Particle::ticks = 0;
	Particle::lifetime = config.get_lifetime();

	int randomsize = config.get_size();
	Particle::size = Vec2(randomsize, randomsize);

	Particle::collides_blocks = true;
	Particle::dead = false;

	Particle::color = config.get_color();

	Particle::shape.setFillColor(Particle::color.toColor());
	Particle::shape.setSize(sf::Vector2f(Particle::size.x, Particle::size.y));
	Particle::shape.setPosition(Particle::pos.x, Particle::pos.y);

	Particle::hasDeathfunc = false;
	Particle::hasUpdatefunc = false;
}

Particle::~Particle() {

}

bool Particle::dead_by_age() {
	return Particle::ticks > Particle::lifetime;
}

void Particle::registerDeath(std::function<void(Particle*)> deathfunc) {
	Particle::hasDeathfunc = true;
	Particle::deathfunc = deathfunc;
}

void Particle::registerUpdate(std::function<void(Particle*)> updatefunc) {
	Particle::hasUpdatefunc = true;
	Particle::updatefunc = updatefunc;
}

void Particle::update(Grid& grid) {
	if (!Particle::hasDeathfunc) {
		if (Particle::dead_by_age()) {
			Particle::dead = true;
		}
	} else {
		Particle::deathfunc(this);
	}
	Particle::ticks++;

	if (!Particle::hasUpdatefunc) {
		Particle::vel.y += 0.35;
		Particle::pos += Particle::vel;
	} else {
		Particle::updatefunc(this);
	}

	Particle::shape.setFillColor(Particle::color.toColor());
	Particle::shape.setSize(sf::Vector2f(Particle::size.x, Particle::size.y));
	Particle::shape.setPosition(Particle::pos.x, Particle::pos.y);
}

void Particle::render(sf::RenderWindow& window, Grid& grid) {
	if (!Particle::dead) {
		Particle::shape.setPosition(pos.x + grid.offset.x, pos.y + grid.offset.y);
		window.draw(Particle::shape);
		Particle::shape.setPosition(pos.x - grid.offset.x, pos.y - grid.offset.y);
	}
}
