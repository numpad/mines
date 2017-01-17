#include "Entity.hpp"

Entity::Entity(const char *skinpath) {
	Entity::pos = Vec2(0, 0);
	Entity::feetOffset = Vec2(0, 0);
	Entity::headOffset = Vec2(0, 0);
	Entity::vel = Vec2(0, 0);
	Entity::lastvel = Vec2(0, 0);
	Entity::acc = Vec2(0, 0);
	Entity::maxvel = Vec2(0, 0);
	Entity::jumpvel = 0;
	Entity::jumpstrength = 0;

	Entity::jumpstate = Entity::JumpState::STANDING;
	Entity::walkstate = Entity::WalkState::STANDING;
	Entity::walkdir   = Entity::WalkState::RIGHT;

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

void Entity::setPos(Vec2 pos) {
	Entity::pos = pos;
}

void Entity::jump(float factor) {
	if (Entity::jumpstate == Entity::JumpState::STANDING) {
		Entity::jumpvel = Entity::jumpstrength * factor;
	}
}

void Entity::walk(Entity::WalkState::Direction dir) {
	Entity::walkstate = Entity::WalkState::WALKING;
	Entity::walkdir = dir;
}

void Entity::physicsHandleJumping(Grid& grid) {
	unsigned int dist_to_ceil_left  = (unsigned int)grid.raycast(Entity::pos + (Entity::headOffset * Vec2(-1.0, 1.0)) + grid.offset, Vec2(0.0, -1.0), 12.0);
	unsigned int dist_to_ceil_right = (unsigned int)grid.raycast(Entity::pos + (Entity::headOffset * Vec2( 1.0, 1.0)) + grid.offset, Vec2(0.0, -1.0), 12.0);
	unsigned int dist_to_ceil = Util::min(dist_to_ceil_left, dist_to_ceil_right);

	if (dist_to_ceil < Entity::jumpvel) {
		/* We hit a block...ouch */
		Entity::jumpvel = 0.0;
		Entity::vel.y = 1.5;
		Entity::jumpstate = Entity::JumpState::FALLING;
	} else {
		Entity::jumpstate = Entity::JumpState::JUMPING;

		Entity::vel.y = Entity::jumpvel;
		Entity::pos.y -= Entity::vel.y;
		Entity::jumpvel *= 0.85;
		if (Entity::jumpvel < 0.25) {
			Entity::jumpstate = Entity::JumpState::FALLING;
			Entity::jumpvel = 0.0;
		}
	}
}

void Entity::physicsHandleFalling(Grid& grid) {
	unsigned int dist_to_floor_left  = (unsigned int)grid.raycast(Entity::pos + (Entity::feetOffset * Vec2(-1.0, 1.0)) + grid.offset, Vec2(0.0, 1.0), 64.0);	
	unsigned int dist_to_floor_right = (unsigned int)grid.raycast(Entity::pos + (Entity::feetOffset * Vec2( 1.0, 1.0)) + grid.offset, Vec2(0.0, 1.0), 64.0);
	unsigned int dist_to_floor = Util::min(dist_to_floor_left, dist_to_floor_right);

	if ((float)dist_to_floor > Entity::vel.y) {
		/* Fall faster! */
		Entity::vel.y += 0.225 + (Entity::vel.y / Entity::maxvel.y) * 0.1;
		/* Limit y-velocity to maximal velocity */
		Util::limit(Entity::vel.y, Entity::maxvel.y);

		/* Update position */
		Entity::pos.y += Entity::vel.y;
		
		/* Update JumpState */
		Entity::jumpstate = Entity::JumpState::FALLING;
	} else {
		/* Move distance to floor, Stop falling */
		Entity::vel.y = 0.0;
		Entity::pos.y += (float)dist_to_floor;

		Entity::jumpstate = Entity::JumpState::STANDING;
	}
}

void Entity::physicsHandleWalking(Grid& grid) {
	/* If standing and movement is close to 0, set to 0 */
	if (Entity::walkstate != Entity::WalkState::WALKING && abs(Entity::vel.x) < 0.001) {
		Entity::vel.x = 0.0;
	}

	/* When walking, update position */
	if (Entity::walkstate == Entity::WalkState::WALKING) {
		Entity::vel.x += Entity::acc.x * (Entity::walkdir == Entity::WalkState::LEFT ? -1.0 : 1.0);
	}
	
	/* Don't move faster than maximum speed */
	if (fabs(Entity::vel.x) > Entity::maxvel.x) {
		const float sign = fabs(Entity::vel.x) / Entity::vel.x;
		Entity::vel.x = Entity::maxvel.x * sign;
	}

	/* If no longer walking, apply friction */
	if (Entity::walkstate != Entity::WalkState::WALKING) {
		Block& standing_on = grid.atPoint(Entity::pos + (Entity::feetOffset * Vec2(0.0, 1.0)) + grid.offset);
		float current_friction = 0.785;
		if (standing_on.id > 0) {
			current_friction = standing_on.friction;
		}
		Entity::vel.x *= current_friction;
	}

	/* Handle walking/collision right */
	const float distance = 16.0;
	const Vec2 dir_right(1.0, 0.0);

	float rays_right[] = {
		grid.raycast(Entity::pos + grid.offset                                                         , dir_right, distance, 9999.0),
		grid.raycast(Entity::pos + grid.offset + (Entity::headOffset * Vec2(0.0, 1.0)) + Vec2(0.0, 2.0), dir_right, distance, 9999.0),
		grid.raycast(Entity::pos + grid.offset + (Entity::feetOffset * Vec2(0.0, 1.0)) - Vec2(0.0, 2.0), dir_right, distance, 9999.0)
	};

	unsigned int rays_right_min = Util::min((unsigned int)rays_right[0], (unsigned int)rays_right[1], (unsigned int)rays_right[2]);

	if (rays_right_min < distance) {
		if (Entity::walkdir == Entity::WalkState::RIGHT) {
			Entity::vel.x = 0;
		}
	}

	/* Handle walking/collision left */
	const Vec2 dir_left(-1.0, 0.0);
	
	float rays_left[] = {
		grid.raycast(Entity::pos + grid.offset                                                         , dir_left, distance, 9999.0),
		grid.raycast(Entity::pos + grid.offset + (Entity::headOffset * Vec2(0.0, 1.0)) + Vec2(0.0, 2.0), dir_left, distance, 9999.0),
		grid.raycast(Entity::pos + grid.offset + (Entity::feetOffset * Vec2(0.0, 1.0)) - Vec2(0.0, 2.0), dir_left, distance, 9999.0)
	};

	unsigned int rays_left_min = Util::min((unsigned int)rays_left[0], (unsigned int)rays_left[1], (unsigned int)rays_left[2]);

	if (rays_left_min < distance) {
		if (Entity::walkdir == Entity::WalkState::LEFT) {
			Entity::vel.x = 0;
		}
	}

	Entity::pos.x += Entity::vel.x;
}

void Entity::physicsUpdate(Grid& grid) {
	
	if (Entity::jumpvel > 0.0) {
		Entity::physicsHandleJumping(grid);
	} else {
		Entity::physicsHandleFalling(grid);
	}

	Entity::physicsHandleWalking(grid);
}