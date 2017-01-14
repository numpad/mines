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
	Entity::vel.x += Entity::acc.x * (dir == Entity::WalkState::LEFT ? -1.0 : 1.0);
	Entity::walkstate = Entity::WalkState::WALKING;
	Entity::walkdir = dir;
}

void Entity::physicsHandleJumping(Grid& grid) {
	unsigned int dist_to_ceil_left  = (unsigned int)grid.raycast(Entity::pos + (Entity::headOffset * Vec2(-1.0, 1.0)) + grid.offset, Vec2(0.0, -1.0), 12.0);
	unsigned int dist_to_ceil_right = (unsigned int)grid.raycast(Entity::pos + (Entity::headOffset * Vec2(-1.0, 1.0)) + grid.offset, Vec2(0.0, -1.0), 12.0);
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
	
	if (Entity::walkstate != Entity::WalkState::WALKING && abs(Entity::vel.x) < 0.001) {
		Entity::vel.x = 0.0;
	}

	if (fabs(Entity::vel.x) > Entity::maxvel.x) {
		const float sign = fabs(Entity::vel.x) / Entity::vel.x;
		Entity::vel.x = Entity::maxvel.x * sign;
	}

	printf("vel.x = %g\n", Entity::vel.x);
	Entity::pos.x += Entity::vel.x;
	Entity::vel.x *= 0.785; // TODO: use friction of block currenty standing on!
	
}

void Entity::physicsUpdate(Grid& grid) {
	if (Entity::jumpvel > 0.0) {
		Entity::physicsHandleJumping(grid);
	} else {
		Entity::physicsHandleFalling(grid);
	}

	Entity::physicsHandleWalking(grid);

	puts("---------------");
	switch (Entity::walkstate) {
		case Entity::WalkState::STANDING:
			printf("Standing");
			break;
		case Entity::WalkState::WALKING:
			printf("Walking ");
			if (Entity::walkdir == Entity::WalkState::LEFT)
				printf("left");
			else
				printf("right");
			break;
	};
	switch (Entity::jumpstate) {
		case Entity::JumpState::FALLING:
			puts("\t | Falling");
			break;
		case Entity::JumpState::JUMPING:
			puts("\t | Jumping");
			break;
		case Entity::JumpState::STANDING:
			puts("\t | Standing");
			break;
		
	};
}