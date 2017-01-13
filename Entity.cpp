#include "Entity.hpp"

Entity::Entity(const char *skinpath) {
	Entity::pos = Vec2(0, 0);
	Entity::feet = Vec2(0, 0);
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
		
	} else {
		unsigned int dist_to_floor_left  = (unsigned int)grid.raycast(Entity::pos + (Entity::feet * Vec2( 1.0, 1.0)) + grid.offset, Vec2(0.0, 1.0), 64.0);
		unsigned int dist_to_floor_right = (unsigned int)grid.raycast(Entity::pos + (Entity::feet * Vec2(-1.0, 1.0)) + grid.offset, Vec2(0.0, 1.0), 64.0);
		unsigned int dist_to_floor = Util::min(dist_to_floor_left, dist_to_floor_right);

		if ((float)dist_to_floor >= Entity::vel.y) {
			/* Fall faster! */
			Entity::vel.y += 0.225 + (Entity::vel.y / Entity::maxvel.y) * 0.1;
			/* Limit y-velocity to maximal velocity */
			Util::limit(Entity::vel.y, Entity::maxvel.y);

			/* Update position */
			Entity::pos.y += Entity::vel.y;
		} else {

		}
	}
}