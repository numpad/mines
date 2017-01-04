#ifndef ENEMY_H
#define ENEMY_H

#include "qw/qw.h"
#include "rgb.h"
#include "rect.h"
#include "vector.h"
#include "block.h"
#include "grid.h"


typedef struct {
	vector_t pos, vel;
	vector_t lastvel;

	float accx;
	float maxvy;
	
	float jumpvel;
	float jumpstrength;

	size_t bodyparts_count;
	rect_t *bodyparts;
	vector_t *bodypartpos;
	vector_t *bodypartanchors;
	float *bodypart_angles;
	SDL_RendererFlip *bodypart_flips;
	qw_image skin;
	
	walk_state walkingstate;
		
	jump_state jumpstate, jumpstate_last;
	walk_direction walkdir;

	int legtimer;

	int selected_blockid;
	int arm_build_timer;
} enemy_t;

void enemy_delete(enemy_t *enemy) {
	free(enemy->bodyparts);
	free(enemy->bodypartpos);
	free(enemy->bodypartanchors);
	free(enemy->bodypart_angles);
	free(enemy->bodypart_flips);
}

enemy_t enemy_new(float x, float y, const char *skinpath) {
	enemy_t enemy;
	enemy.walkdir = WALKDIR_LEFT;
	enemy.walkingstate = STANDING;
	enemy.jumpstate = ONGROUND;
	enemy.jumpstate_last = ONGROUND;

	enemy.pos = vector_new(x, y);
	
	enemy.vel = vector_new(0, 0);
	enemy.lastvel = vector_new(0, 0);

	enemy.accx = 0.45;
	enemy.maxvy = 7.75;

	enemy.jumpvel = 0;
	enemy.jumpstrength = 6.2;

	enemy.bodyparts_count = 6;
	/* sprite positions and dimensions */
	enemy.bodyparts = malloc(enemy.bodyparts_count * sizeof(rect_t));
	enemy.bodyparts[HEAD] = rect_new(5, 5, 22, 22);
	enemy.bodyparts[BODY] = rect_new(5, 29, 8, 24);
	enemy.bodyparts[ARMF] = rect_new(19, 29, 8, 24);
	enemy.bodyparts[ARMB] = rect_new(33, 29, 8, 24);
	enemy.bodyparts[LEGF] = rect_new(33, 3, 8, 24);
	enemy.bodyparts[LEGB] = rect_new(46, 3, 8, 24);
	/* bodypart anchors [0..1] */
	enemy.bodypartanchors = malloc(enemy.bodyparts_count * sizeof(vector_t));
	enemy.bodypartanchors[HEAD] = vector_new(0.5, 0.9);
	enemy.bodypartanchors[BODY] = vector_new(0.5, 0.5);
	enemy.bodypartanchors[ARMF] = vector_new(0.5, 0.1);
	enemy.bodypartanchors[ARMB] = vector_new(0.5, 0.1);
	enemy.bodypartanchors[LEGF] = vector_new(0.5, 0.1);
	enemy.bodypartanchors[LEGB] = vector_new(0.5, 0.1);
	/* real world coordinates relative to enemy pos */
	enemy.bodypartpos = malloc(enemy.bodyparts_count * sizeof(vector_t));
	enemy.bodypartpos[HEAD] = vector_new(0, -46);
	enemy.bodypartpos[BODY] = vector_new(0, -36);
	enemy.bodypartpos[ARMF] = vector_new(0, -42);
	enemy.bodypartpos[ARMB] = vector_new(0, -42);
	enemy.bodypartpos[LEGF] = vector_new(0, -22);
	enemy.bodypartpos[LEGB] = vector_new(0, -22);
	/* bodypart rotations */
	enemy.bodypart_angles = malloc(enemy.bodyparts_count * sizeof(float));
	enemy.bodypart_angles[HEAD] = 0;
	enemy.bodypart_angles[BODY] = 0;
	enemy.bodypart_angles[ARMF] = 30;
	enemy.bodypart_angles[ARMB] = -30;
	enemy.bodypart_angles[LEGF] = 20;
	enemy.bodypart_angles[LEGB] = -20;
	/* bodypart flips */
	enemy.bodypart_flips = calloc(enemy.bodyparts_count, sizeof(SDL_RendererFlip));

	enemy.legtimer = 0;
	enemy.arm_build_timer = 0;
	enemy.selected_blockid = 10;

	enemy.skin = qw_loadimage(skinpath);
	return enemy;
}

void enemy_jump(player_t *enemy) {
	enemy->jumpvel = enemy->jumpstrength;
}

void enemy_walk(enemy_t *enemy, walk_direction dir) {
	enemy->vel.x += enemy->accx * (dir == WALKDIR_LEFT ? -1.0 : 1.0);
	enemy->walkingstate = WALKING;
	enemy->walkdir = dir ;
}

void enemy_draw(enemy_t *enemy, grid_t grid) {
	float sitting_height = 0;
	if (enemy->walkingstate == SITTING) {
		sitting_height = grid.blocksize / 2.0;
	}

	float leftfoot = enemy->pos.x + grid.xoff - grid.blocksize / 3.0;
	float rightfoot = enemy->pos.x + grid.xoff + grid.blocksize / 3.0;
	
	DEBUG_BEGIN
		/* render full enemy size */
		qw_color(255, 0, 255, 255);
		qw_drawrect(enemy->pos.x - grid.blocksize / 2 + grid.xoff, enemy->pos.y - grid.blocksize * 2 + grid.yoff, grid.blocksize, grid.blocksize * 2);
		/* render enemy collision size */
		qw_color(0, 100, 150, 255);
		qw_drawrect(enemy->pos.x - grid.blocksize / 3.0 + grid.xoff, enemy->pos.y - grid.blocksize * 2 + grid.yoff, (grid.blocksize / 3.0) * 2.0 + 1.0, grid.blocksize * 2);
		
		/* render enemy feet */
		qw_color(0, 255, 0, 255);
		qw_fillcircle(leftfoot, enemy->pos.y + grid.yoff - sitting_height, 3);
		qw_fillcircle(rightfoot, enemy->pos.y + grid.yoff - sitting_height, 3);
	DEBUG_END

	for (int i = 0; i < enemy->bodyparts_count; ++i) {
		const vector_t pos = vector_add(enemy->bodypartpos[i], vector_new(grid.xoff, grid.yoff));
		const rect_t part = enemy->bodyparts[i];
		vector_t anchor = enemy->bodypartanchors[i];
		const float angle = enemy->bodypart_angles[i];
		
		if (enemy->bodypart_flips[i] == SDL_FLIP_VERTICAL) {
			anchor.y = 1.0 - anchor.y;
		}
	
		const float wa = part.w * anchor.x,
		            ha = part.h * anchor.y;
		
		qw_drawimage_quadrot(enemy->skin, enemy->pos.x + pos.x - wa, enemy->pos.y + pos.y - ha, part.w, part.h, part.x, part.y, part.w, part.h, angle, wa, ha, enemy->bodypart_flips[i]);
		
		DEBUG_BEGIN
			/* draw body anchors */
			qw_color(255, 0, 0, 255);
			qw_drawcircle(enemy->pos.x + pos.x, enemy->pos.y + pos.y, 3);
		DEBUG_END
	}
	
	DEBUG_BEGIN
		/* draw raycasts down */
		qw_color(0, 255, 0, 255);
		qw_drawline(leftfoot, enemy->pos.y + grid.yoff, leftfoot, enemy->pos.y + enemy->maxvy * 2 + grid.yoff);
		qw_color(0, 255, 0, 255);
		qw_drawline(rightfoot, enemy->pos.y + grid.yoff, rightfoot, enemy->pos.y + enemy->maxvy * 2 + grid.yoff);
	
		/* draw raycasts right */
		qw_color(255, 0, 0, 255);
		qw_drawline(enemy->pos.x + grid.blocksize / 3 + grid.xoff, enemy->pos.y - 2.0 + grid.yoff, enemy->pos.x + grid.blocksize / 3 + 6.0 + grid.xoff, enemy->pos.y - 2.0 + grid.yoff);
		qw_drawline(enemy->pos.x + grid.blocksize / 3 + grid.xoff, enemy->pos.y - grid.blocksize + grid.yoff, enemy->pos.x + grid.blocksize / 3 + 6.0 + grid.xoff, enemy->pos.y - grid.blocksize + grid.yoff);
		qw_drawline(enemy->pos.x + grid.blocksize / 3 + grid.xoff, enemy->pos.y - grid.blocksize * 2.0 + 2.0 + grid.yoff, enemy->pos.x + grid.blocksize / 3 + 6.0 + grid.xoff, enemy->pos.y - grid.blocksize * 2.0 + 2.0 + grid.yoff);
	DEBUG_END
}

void enemy_update(enemy_t *enemy, grid_t *grid) {
	/* head rotation */
	vector_t mousepos = vector_new(qw_mousex, qw_mousey);
	vector_t headpos = vector_new(enemy->pos.x + enemy->bodypartpos[HEAD].x + grid->xoff,
	                              enemy->pos.y + enemy->bodypartpos[HEAD].y + grid->yoff - (enemy->bodyparts[HEAD].h / 2.0));
	
	enemy->bodypart_angles[HEAD] = vector_angle(headpos, mousepos);
	/* flip the head when looking at the left side */
	if (abs(enemy->bodypart_angles[HEAD]) > 90 && abs(enemy->bodypart_angles[HEAD] < 180)) {
		enemy->bodypart_flips[HEAD] = SDL_FLIP_VERTICAL;
		enemy->bodypart_flips[LEGF] = SDL_FLIP_HORIZONTAL;
		enemy->bodypart_flips[LEGB] = SDL_FLIP_HORIZONTAL;
		enemy->bodypart_flips[ARMF] = SDL_FLIP_HORIZONTAL;
		enemy->bodypart_flips[ARMB] = SDL_FLIP_HORIZONTAL;
		enemy->bodypart_flips[BODY] = SDL_FLIP_HORIZONTAL;
	} else {
		enemy->bodypart_flips[HEAD] = SDL_FLIP_NONE;
		enemy->bodypart_flips[LEGF] = SDL_FLIP_NONE;
		enemy->bodypart_flips[LEGB] = SDL_FLIP_NONE;
		enemy->bodypart_flips[ARMF] = SDL_FLIP_NONE;
		enemy->bodypart_flips[ARMB] = SDL_FLIP_NONE;
		enemy->bodypart_flips[BODY] = SDL_FLIP_NONE;
	}
	
	/* jumping, collision */
	if (enemy->jumpvel > 0) {
		
		/* collision with block(s) above */
		int dist_to_ceil_left = raycast_up(*grid, enemy->pos.x + grid->xoff - grid->blocksize / 3.0, enemy->pos.y - grid->blocksize * 2.0 + grid->yoff, enemy->jumpvel);
		int dist_to_ceil_right = raycast_up(*grid, enemy->pos.x + grid->xoff + grid->blocksize / 3.0, enemy->pos.y - grid->blocksize * 2.0 + grid->yoff, enemy->jumpvel);
		if ((unsigned int)dist_to_ceil_left < enemy->jumpvel || (unsigned int)dist_to_ceil_right < enemy->jumpvel) {
			/* we hit a block...ouch */
			enemy->jumpvel = 0;
			enemy->vel.y = 1.5;
			enemy->jumpstate = FALLING;
		} else {
			enemy->jumpstate = JUMPING;
			/* no block close enough above us */
			enemy->vel.y = enemy->jumpvel;
			enemy->pos.y -= enemy->vel.y;
			enemy->jumpvel *= 0.85;
			if (enemy->jumpvel < 0.25) {
				enemy->jumpstate = FALLING;
				enemy->jumpvel = 0;
			}
		}
	} else {
		
		/* falling down, collision checks with floor */
		int dist_to_floor_left = raycast_down(*grid, enemy->pos.x + grid->xoff - grid->blocksize / 3.0, enemy->pos.y + grid->yoff, enemy->maxvy * 2);
		int dist_to_floor_right = raycast_down(*grid, enemy->pos.x + grid->xoff + grid->blocksize / 3.0, enemy->pos.y + grid->yoff, enemy->maxvy * 2);

		if ((unsigned int)dist_to_floor_left > enemy->vel.y && (unsigned int)dist_to_floor_right > enemy->vel.y) {
			//enemy->vel.y += 0.225;
			enemy->vel.y += 0.225 + (enemy->vel.y / enemy->maxvy) * 0.1;

			if (enemy->vel.y > enemy->maxvy)
				enemy->vel.y = enemy->maxvy;
			enemy->pos.y += enemy->vel.y;

			enemy->jumpstate = FALLING;
		} else {
			/* we will have fallen to far next frame -> set appropriate position */
			enemy->pos.y += fmax(dist_to_floor_left, dist_to_floor_right);
			enemy->vel.y = 0;
			enemy->jumpstate = ONGROUND;
		}
	}
	
	const int coll_distance = 6;

	/* raycasts right */
	int collision_right[3] = {
		raycast_right(*grid, enemy->pos.x + grid->blocksize / 3.0 + grid->xoff, enemy->pos.y - 2.0 + grid->yoff, coll_distance),
		raycast_right(*grid, enemy->pos.x + grid->blocksize / 3.0 + grid->xoff, enemy->pos.y - grid->blocksize + grid->yoff, coll_distance),
		raycast_right(*grid, enemy->pos.x + grid->blocksize / 3.0 + grid->xoff, enemy->pos.y - grid->blocksize * 2.0 + 2.0 + grid->yoff, coll_distance)
	};
	for (int i = 0; i < 3; ++i) {
		if ((unsigned int)collision_right[i] < coll_distance) {
			if (enemy->walkdir == WALKDIR_RIGHT) {
				enemy->pos.x += collision_right[i] - 1.0;
				enemy->pos.x = floor(enemy->pos.x);
				enemy->vel.x = 0;
				enemy->walkingstate = STANDING;
			}
			break;
		}
	}
	
	/* raycasts left */
	int collision_left[3] = {
		raycast_left(*grid, enemy->pos.x - grid->blocksize / 3.0 + grid->xoff, enemy->pos.y - 2.0 + grid->yoff, coll_distance),
		raycast_left(*grid, enemy->pos.x - grid->blocksize / 3.0 + grid->xoff, enemy->pos.y - grid->blocksize + grid->yoff, coll_distance),
		raycast_left(*grid, enemy->pos.x - grid->blocksize / 3.0 + grid->xoff, enemy->pos.y - grid->blocksize * 2.0 + 2.0 + grid->yoff, coll_distance)
	};
	for (int i = 0; i < 3; ++i) {
		if ((unsigned int)collision_left[i] < coll_distance) {
			if (enemy->walkdir == WALKDIR_LEFT) {
				enemy->pos.x -= collision_left[i] - 1.0;
				enemy->pos.x = floor(enemy->pos.x);
				enemy->vel.x = 0;
				enemy->walkingstate = STANDING;
			}
			break;
		}
	}
	
	/* x axis movement */
	if (abs(enemy->vel.x) > 3.5) {
		float sign = abs(enemy->vel.x) / enemy->vel.x;
		enemy->vel.x = 3.5 * sign;
	}

	/* postion update */
	enemy->pos.x += enemy->vel.x;

	/* drag */
	enemy->vel.x *= 0.785;
	if (enemy->walkingstate != WALKING && abs(enemy->vel.x) < 0.00001)
		enemy->vel.x = 0;
	
	/* animations */
	if (enemy->walkingstate == WALKING) {
		enemy->legtimer += (enemy->walkdir == 0) ? 1 : -1;
		enemy->bodypart_angles[LEGF] = sin(enemy->legtimer / 11.0) * 30;
		enemy->bodypart_angles[LEGB] = sin(enemy->legtimer / 11.0) * -30;
		
		enemy->bodypart_angles[ARMF] = sin(enemy->legtimer / 9.0) * 6 + (90.0 * (enemy->walkdir == WALKDIR_LEFT ? 1.0 : -1.0));
		enemy->bodypart_angles[ARMB] = sin(enemy->legtimer / 9.0) * -6 + (90.0 * (enemy->walkdir == WALKDIR_LEFT ? 1.0 : -1.0));
	} else if (enemy->walkingstate == STANDING) {
		if (abs(enemy->bodypart_angles[LEGB]) > 6) {
			enemy->legtimer += (enemy->walkdir == 0) ? 2 : -2;
			enemy->bodypart_angles[LEGF] = sin(enemy->legtimer / 5.0) * 30;
			enemy->bodypart_angles[LEGB] = sin(enemy->legtimer / 5.0) * -30;
		}
	} else if (enemy->walkingstate == SITTING) {
		enemy->bodypart_angles[LEGF] = 85.0 * (enemy->bodypart_flips[HEAD] == SDL_FLIP_NONE ? -1 : 1);
		enemy->bodypart_angles[LEGB] = 94.0 * (enemy->bodypart_flips[HEAD] == SDL_FLIP_NONE ? -1 : 1);
	}
	
	/* arm movement */
	if (enemy->arm_build_timer > 0) {
		vector_t mousepos = vector_new(qw_mousex, qw_mousey);
		vector_t armfpos = vector_new(enemy->pos.x + enemy->bodypartpos[ARMF].x + grid->xoff - (enemy->bodyparts[ARMF].w * enemy->bodypartanchors[ARMF].x),
	    	                          enemy->pos.y + enemy->bodypartpos[ARMF].y + grid->yoff - (enemy->bodyparts[ARMF].h * enemy->bodypartanchors[ARMF].y));
		float angle = vector_angle(armfpos, mousepos);

		enemy->bodypart_angles[ARMF] = (angle - 90.0) + sin(enemy->arm_build_timer / 2.0) * 25.0;
		enemy->arm_build_timer--;
	}

	enemy->walkingstate = STANDING;
}


void enemy_updatelast(enemy_t *enemy, grid_t *grid) {
	/* update jumpstate_last */
	enemy->jumpstate_last = enemy->jumpstate;
	enemy->lastvel = enemy->vel;
}


#endif
