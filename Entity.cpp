#include "Entity.hpp"

Entity::Entity(const char *skinpath) {
	Entity::pos = Vec2(0, 0);
	Entity::vel = Vec2(0, 0);
	Entity::lastvel = Vec2(0, 0);
	Entity::acc = Vec2(0, 0);
	Entity::maxvel = Vec2(0, 0);
	Entity::jumpvel = 0;
	Entity::jumpstrength = 0;
	if (!Entity::skin.loadFromFile(skinpath)) {
		puts("could not load entity skin");
	}
}

void Entity::addLimb(Limb body) {
	Entity::body = body;
}

void Entity::addChildLimb(Limb body) {
	Entity::body.addChild(body);
}

void Entity::physicsUpdate(Grid& grid) {
	if (Entity::jumpvel > 0.0) {
		
	}
}