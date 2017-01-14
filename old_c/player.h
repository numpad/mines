#ifndef PLAYER_H
#define PLAYER_H

#include "qw/qw.h"
#include "rgb.h"
#include "rect.h"
#include "vector.h"
#include "block.h"
#include "grid.h"

typedef enum {
	ARMB, LEGB, LEGF, BODY, HEAD, ARMF
} bodypart;

typedef enum {
	WALKDIR_LEFT, WALKDIR_RIGHT
} walk_direction;

	
typedef enum {
	STANDING, WALKING, SITTING
} walk_state;

typedef enum {
	JUMPING, FALLING, ONGROUND
} jump_state;

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
	
	jump_state jumpstate, jumpstate_last;
	
	walk_state walkingstate;
	walk_direction walkdir;

	int legtimer;

	int selected_blockid;
	int arm_build_timer;
} player_t;

void player_delete(player_t *player) {
	free(player->bodyparts);
	free(player->bodypartpos);
	free(player->bodypartanchors);
	free(player->bodypart_angles);
	free(player->bodypart_flips);
}

player_t player_new(float x, float y, const char *skinpath) {
	player_t player;
	player.walkdir = WALKDIR_LEFT;
	player.walkingstate = STANDING;
	player.jumpstate = ONGROUND;
	player.jumpstate_last = ONGROUND;

	player.pos = vector_new(x, y);
	
	player.vel = vector_new(0, 0);
	player.lastvel = vector_new(0, 0);
	
	player.accx = 0.75;
	player.maxvy = 7.75;

	player.jumpvel = 0;
	player.jumpstrength = 6.2;

	player.bodyparts_count = 6;
	/* sprite positions and dimensions */
	player.bodyparts = malloc(player.bodyparts_count * sizeof(rect_t));
	player.bodyparts[HEAD] = rect_new(5, 5, 22, 22);
	player.bodyparts[BODY] = rect_new(5, 29, 8, 24);
	player.bodyparts[ARMF] = rect_new(19, 29, 8, 24);
	player.bodyparts[ARMB] = rect_new(33, 29, 8, 24);
	player.bodyparts[LEGF] = rect_new(33, 3, 8, 24);
	player.bodyparts[LEGB] = rect_new(46, 3, 8, 24);
	/* bodypart anchors [0..1] */
	player.bodypartanchors = malloc(player.bodyparts_count * sizeof(vector_t));
	player.bodypartanchors[HEAD] = vector_new(0.5, 0.9);
	player.bodypartanchors[BODY] = vector_new(0.5, 0.5);
	player.bodypartanchors[ARMF] = vector_new(0.5, 0.1);
	player.bodypartanchors[ARMB] = vector_new(0.5, 0.1);
	player.bodypartanchors[LEGF] = vector_new(0.5, 0.1);
	player.bodypartanchors[LEGB] = vector_new(0.5, 0.1);
	/* real world coordinates relative to player pos */
	player.bodypartpos = malloc(player.bodyparts_count * sizeof(vector_t));
	player.bodypartpos[HEAD] = vector_new(0, -46);
	player.bodypartpos[BODY] = vector_new(0, -36);
	player.bodypartpos[ARMF] = vector_new(0, -42);
	player.bodypartpos[ARMB] = vector_new(0, -42);
	player.bodypartpos[LEGF] = vector_new(0, -22);
	player.bodypartpos[LEGB] = vector_new(0, -22);
	/* bodypart rotations */
	player.bodypart_angles = malloc(player.bodyparts_count * sizeof(float));
	player.bodypart_angles[HEAD] = 0;
	player.bodypart_angles[BODY] = 0;
	player.bodypart_angles[ARMF] = 30;
	player.bodypart_angles[ARMB] = -30;
	player.bodypart_angles[LEGF] = 20;
	player.bodypart_angles[LEGB] = -20;
	/* bodypart flips */
	player.bodypart_flips = calloc(player.bodyparts_count, sizeof(SDL_RendererFlip));

	player.legtimer = 0;
	player.arm_build_timer = 0;
	player.selected_blockid = 10;

	player.skin = qw_loadimage(skinpath);
	return player;
}

void player_jump(player_t *player) {
	player->jumpvel = player->jumpstrength;
}

void player_walk(player_t *player, walk_direction dir) {
	player->vel.x += player->accx * (dir == WALKDIR_LEFT ? -1.0 : 1.0);
	player->walkingstate = WALKING;
	player->walkdir = dir;
}

void player_draw(player_t *player, grid_t grid) {
	float sitting_height = 0;
	if (player->walkingstate == SITTING) {
		sitting_height = grid.blocksize / 2.0;
	}

	float leftfoot = player->pos.x + grid.xoff - grid.blocksize / 3.0;
	float rightfoot = player->pos.x + grid.xoff + grid.blocksize / 3.0;
	
	DEBUG_BEGIN
		/* render full player size */
		qw_color(255, 0, 255, 255);
		qw_drawrect(player->pos.x - grid.blocksize / 2 + grid.xoff, player->pos.y - grid.blocksize * 2 + grid.yoff, grid.blocksize, grid.blocksize * 2);
		/* render player collision size */
		qw_color(0, 100, 150, 255);
		qw_drawrect(player->pos.x - grid.blocksize / 3.0 + grid.xoff, player->pos.y - grid.blocksize * 2 + grid.yoff, (grid.blocksize / 3.0) * 2.0 + 1.0, grid.blocksize * 2);
		
		/* render player feet */
		qw_color(0, 255, 0, 255);
		qw_fillcircle(leftfoot, player->pos.y + grid.yoff - sitting_height, 3);
		qw_fillcircle(rightfoot, player->pos.y + grid.yoff - sitting_height, 3);
	DEBUG_END

	for (int i = 0; i < player->bodyparts_count; ++i) {
		const vector_t pos = vector_add(player->bodypartpos[i], vector_new(grid.xoff, grid.yoff));
		const rect_t part = player->bodyparts[i];
		vector_t anchor = player->bodypartanchors[i];
		const float angle = player->bodypart_angles[i];
		
		if (player->bodypart_flips[i] == SDL_FLIP_VERTICAL) {
			anchor.y = 1.0 - anchor.y;
		}
	
		const float wa = part.w * anchor.x,
		            ha = part.h * anchor.y;
		
		qw_drawimage_quadrot(player->skin, player->pos.x + pos.x - wa, player->pos.y + pos.y - ha, part.w, part.h, part.x, part.y, part.w, part.h, angle, wa, ha, player->bodypart_flips[i]);
		
		DEBUG_BEGIN
			/* draw body anchors */
			qw_color(255, 0, 0, 255);
			qw_drawcircle(player->pos.x + pos.x, player->pos.y + pos.y, 3);
		DEBUG_END
	}
	
	DEBUG_BEGIN
		/* draw raycasts down */
		qw_color(0, 255, 0, 255);
		qw_drawline(leftfoot, player->pos.y + grid.yoff, leftfoot, player->pos.y + player->maxvy * 2 + grid.yoff);
		qw_color(0, 255, 0, 255);
		qw_drawline(rightfoot, player->pos.y + grid.yoff, rightfoot, player->pos.y + player->maxvy * 2 + grid.yoff);
		
		/* draw raycasts right */
		qw_color(255, 0, 0, 255);
		qw_drawline(player->pos.x + grid.blocksize / 3 + grid.xoff, player->pos.y - 2.0 + grid.yoff, player->pos.x + grid.blocksize / 3 + 6.0 + grid.xoff, player->pos.y - 2.0 + grid.yoff);
		qw_drawline(player->pos.x + grid.blocksize / 3 + grid.xoff, player->pos.y - grid.blocksize + grid.yoff, player->pos.x + grid.blocksize / 3 + 6.0 + grid.xoff, player->pos.y - grid.blocksize + grid.yoff);
		qw_drawline(player->pos.x + grid.blocksize / 3 + grid.xoff, player->pos.y - grid.blocksize * 2.0 + 2.0 + grid.yoff, player->pos.x + grid.blocksize / 3 + 6.0 + grid.xoff, player->pos.y - grid.blocksize * 2.0 + 2.0 + grid.yoff);
	DEBUG_END
}

void player_update(player_t *player, grid_t *grid) {
	/* head rotation */
	vector_t mousepos = vector_new(qw_mousex, qw_mousey);
	vector_t headpos = vector_new(player->pos.x + player->bodypartpos[HEAD].x + grid->xoff,
	                              player->pos.y + player->bodypartpos[HEAD].y + grid->yoff - (player->bodyparts[HEAD].h / 2.0));
	
	player->bodypart_angles[HEAD] = vector_angle(headpos, mousepos);
	/* flip the head when looking at the left side */
	if (abs(player->bodypart_angles[HEAD]) > 90 && abs(player->bodypart_angles[HEAD] < 180)) {
		player->bodypart_flips[HEAD] = SDL_FLIP_VERTICAL;
		player->bodypart_flips[LEGF] = SDL_FLIP_HORIZONTAL;
		player->bodypart_flips[LEGB] = SDL_FLIP_HORIZONTAL;
		player->bodypart_flips[ARMF] = SDL_FLIP_HORIZONTAL;
		player->bodypart_flips[ARMB] = SDL_FLIP_HORIZONTAL;
		player->bodypart_flips[BODY] = SDL_FLIP_HORIZONTAL;
	} else {
		player->bodypart_flips[HEAD] = SDL_FLIP_NONE;
		player->bodypart_flips[LEGF] = SDL_FLIP_NONE;
		player->bodypart_flips[LEGB] = SDL_FLIP_NONE;
		player->bodypart_flips[ARMF] = SDL_FLIP_NONE;
		player->bodypart_flips[ARMB] = SDL_FLIP_NONE;
		player->bodypart_flips[BODY] = SDL_FLIP_NONE;
	}
	
	/* jumping, collision */
	if (player->jumpvel > 0) {
		
		/* collision with block(s) above */
		int dist_to_ceil_left = raycast_up(*grid, player->pos.x + grid->xoff - grid->blocksize / 3.0, player->pos.y - grid->blocksize * 2.0 + grid->yoff, player->jumpvel);
		int dist_to_ceil_right = raycast_up(*grid, player->pos.x + grid->xoff + grid->blocksize / 3.0, player->pos.y - grid->blocksize * 2.0 + grid->yoff, player->jumpvel);
		if ((unsigned int)dist_to_ceil_left < player->jumpvel || (unsigned int)dist_to_ceil_right < player->jumpvel) {
			/* we hit a block...ouch */
			player->jumpvel = 0;
			player->vel.y = 1.5;
			player->jumpstate = FALLING;
		} else {
			player->jumpstate = JUMPING;
			/* no block close enough above us */
			player->vel.y = player->jumpvel;
			player->pos.y -= player->vel.y;
			player->jumpvel *= 0.85;
			if (player->jumpvel < 0.25) {
				player->jumpstate = FALLING;
				player->jumpvel = 0;
			}
		}
	} else {
		
		/* falling down, collision checks with floor */
		int dist_to_floor_left = raycast_down(*grid, player->pos.x + grid->xoff - grid->blocksize / 3.0, player->pos.y + grid->yoff, player->maxvy * 2);
		int dist_to_floor_right = raycast_down(*grid, player->pos.x + grid->xoff + grid->blocksize / 3.0, player->pos.y + grid->yoff, player->maxvy * 2);

		if ((unsigned int)dist_to_floor_left > player->vel.y && (unsigned int)dist_to_floor_right > player->vel.y) {
			//player->vel.y += 0.225;
			player->vel.y += 0.225 + (player->vel.y / player->maxvy) * 0.1;

			if (player->vel.y > player->maxvy)
				player->vel.y = player->maxvy;
			player->pos.y += player->vel.y;

			player->jumpstate = FALLING;
		} else {
			/* we will have fallen to far next frame -> set appropriate position */
			player->pos.y += fmax(dist_to_floor_left, dist_to_floor_right);
			player->vel.y = 0;
			player->jumpstate = ONGROUND;
		}
	}
	
	const int coll_distance = 6;

	/* raycasts right */
	int collision_right[3] = {
		raycast_right(*grid, player->pos.x + grid->blocksize / 3.0 + grid->xoff, player->pos.y - 2.0 + grid->yoff, coll_distance),
		raycast_right(*grid, player->pos.x + grid->blocksize / 3.0 + grid->xoff, player->pos.y - grid->blocksize + grid->yoff, coll_distance),
		raycast_right(*grid, player->pos.x + grid->blocksize / 3.0 + grid->xoff, player->pos.y - grid->blocksize * 2.0 + 2.0 + grid->yoff, coll_distance)
	};
	for (int i = 0; i < 3; ++i) {
		if ((unsigned int)collision_right[i] < coll_distance) {
			if (player->walkdir == WALKDIR_RIGHT) {
				player->pos.x += collision_right[i] - 1.0;
				player->pos.x = floor(player->pos.x);
				player->vel.x = 0;
				player->walkingstate = STANDING;
			}
			break;
		}
	}
	
	/* raycasts left */
	int collision_left[3] = {
		raycast_left(*grid, player->pos.x - grid->blocksize / 3.0 + grid->xoff, player->pos.y - 2.0 + grid->yoff, coll_distance),
		raycast_left(*grid, player->pos.x - grid->blocksize / 3.0 + grid->xoff, player->pos.y - grid->blocksize + grid->yoff, coll_distance),
		raycast_left(*grid, player->pos.x - grid->blocksize / 3.0 + grid->xoff, player->pos.y - grid->blocksize * 2.0 + 2.0 + grid->yoff, coll_distance)
	};
	for (int i = 0; i < 3; ++i) {
		if ((unsigned int)collision_left[i] < coll_distance) {
			if (player->walkdir == WALKDIR_LEFT) {
				player->pos.x -= collision_left[i] - 1.0;
				player->pos.x = floor(player->pos.x);
				player->vel.x = 0;
				player->walkingstate = STANDING;
			}
			break;
		}
	}
	
	/* x axis movement */
	if (abs(player->vel.x) > 3.5) {
		float sign = abs(player->vel.x) / player->vel.x;
		player->vel.x = 3.5 * sign;
	}

	/* postion update */
	player->pos.x += player->vel.x;

	/* drag */
	player->vel.x *= 0.785;
	if (player->walkingstate != WALKING && abs(player->vel.x) < 0.00001)
		player->vel.x = 0;
	
	/* animations */
	if (player->walkingstate == WALKING) {
		player->legtimer += (player->walkdir == 0) ? 1 : -1;
		player->bodypart_angles[LEGF] = sin(player->legtimer / 4.0) * 30;
		player->bodypart_angles[LEGB] = sin(player->legtimer / 4.0) * -30;
		
		player->bodypart_angles[ARMF] = sin(player->legtimer / 5.0) * 20;
		player->bodypart_angles[ARMB] = sin(player->legtimer / 5.0) * -20;
	} else if (player->walkingstate == STANDING) {
		if (abs(player->bodypart_angles[LEGB]) > 6) {
			player->legtimer += (player->walkdir == 0) ? 2 : -2;
			player->bodypart_angles[LEGF] = sin(player->legtimer / 4.0) * 30;
			player->bodypart_angles[LEGB] = sin(player->legtimer / 4.0) * -30;
		}
	} else if (player->walkingstate == SITTING) {
		player->bodypart_angles[LEGF] = 85.0 * (player->bodypart_flips[HEAD] == SDL_FLIP_NONE ? -1 : 1);
		player->bodypart_angles[LEGB] = 94.0 * (player->bodypart_flips[HEAD] == SDL_FLIP_NONE ? -1 : 1);
	}
	
	/* arm movement */
	if (player->arm_build_timer > 0) {
		vector_t mousepos = vector_new(qw_mousex, qw_mousey);
		vector_t armfpos = vector_new(player->pos.x + player->bodypartpos[ARMF].x + grid->xoff - (player->bodyparts[ARMF].w * player->bodypartanchors[ARMF].x),
	    	                          player->pos.y + player->bodypartpos[ARMF].y + grid->yoff - (player->bodyparts[ARMF].h * player->bodypartanchors[ARMF].y));
		float angle = vector_angle(armfpos, mousepos);

		player->bodypart_angles[ARMF] = (angle - 90.0) + sin(player->arm_build_timer / 2.0) * 25.0;
		player->arm_build_timer--;
	}
}


void player_updatelast(player_t *player, grid_t *grid) {
	/* update jumpstate_last */
	player->jumpstate_last = player->jumpstate;
	player->lastvel = player->vel;
}

#endif
