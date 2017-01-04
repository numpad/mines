#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG_LEVEL 1
#define DEBUG(A) do{A;}while(DEBUG_LEVEL)
#define DEBUG_BEGIN if(DEBUG_LEVEL){
#define DEBUG_END }

#include "qw/qw.h"
#include "rgb.h"
#include "rect.h"
#include "vector.h"
#include "block.h"
#include "grid.h"
#include "util.h"
#include "particle.h"
#include "worldgen.h"
#include "player.h"
#include "enemy.h"

void drawstars(int xoff, int yoff, int seed) {
	srand(seed);
	
	qw_color(255, 255, 255, 255);
	for (int y = 0; y < qw_height; y += rand() % 30 + 30) {
		for (int x = 0; x < qw_width; x += rand() % 30 + 30) {
			if (rand() % 100 < 30)
				continue;

			const int sz = rand() % 3 + 1;
			int xp = x + rand() % 30 - 15 + xoff;
			int yp = y + rand() % 30 - 15 + yoff;
			
			while (xp < 0) {
				xp += qw_width;
			}

			qw_fillrect(xp, yp, sz, sz);
		}
	}
	srand(time(0));
}

int main(int argc, char *argv[]) {
	qw_screen(800, 740, 0, "Mines");
	
	grid_t grid = grid_loadfrom("world0.sav");
	if (grid.blocksize < 0) {
		grid.blocksize = 32;
		genworld(grid);
	}

	player_t player = player_new(250, 500, "assets/skin.png");
	
	float daytime = 0,
	      daylength = 7500;
	int sunrise = 1;
	unsigned int days = 0;

	/* particles */
	particle_preset_t particle_blood = particle_preset_new(7,
	                                       /* lifetime: */ 80, 170,
										   /* size: */      2,  6);
	particle_preset_addcolor(&particle_blood, rgb_new(194, 48, 48));
	particle_preset_addcolor(&particle_blood, rgb_new(217, 95, 95));
	particle_preset_addcolor(&particle_blood, rgb_new(236, 46, 46));
	particle_preset_addcolor(&particle_blood, rgb_new(171, 24, 24));
	particle_preset_addcolor(&particle_blood, rgb_new(167, 65, 65));
	particle_preset_addcolor(&particle_blood, rgb_new(133, 12, 12));
	particle_preset_addcolor(&particle_blood, rgb_new(232, 65, 65));
	particle_blood.type = PARTICLE_FLUID;
	particle_blood.deathtype = PARTICLEDEATH_FADEOUT;
	particle_t particles[32];
	size_t particles_count = 0;
	
	enemy_t zombie0 = enemy_new(450, 400, "assets/enemies/zombie2.png");

	time_t starttime = time(0);
	srand(starttime);
	while (qw_running()) {
		if (qw_keydown(QW_KEY(ESCAPE)))
			qw_quit();
		
		float darkness = (daytime / daylength);
		qw_fill(61 * darkness, 159 * darkness, 203 * darkness);
		
		/* draw stars */
		drawstars(grid.xoff / 10, grid.yoff / 10, days);

		/* input */
		if (qw_keydown(QW_KEY(W)) && player.jumpvel == 0 && player.jumpstate == ONGROUND) {
			player_jump(&player);
		}
		if (qw_keydown(QW_KEY(A))) {
			if (player.walkingstate != SITTING) {
				player_walk(&player, WALKDIR_LEFT);
			}
			
			enemy_walk(&zombie0, WALKDIR_LEFT);
		}
		if (qw_keydown(QW_KEY(D))) {
			if (player.walkingstate != SITTING) {
				player_walk(&player, WALKDIR_RIGHT);
			}

			enemy_walk(&zombie0, WALKDIR_RIGHT);
		}

		if (!qw_keydown(QW_KEY(A)) && ! qw_keydown(QW_KEY(D))) {
			player.walkingstate = STANDING;
		}

		if (qw_keydown(QW_KEY(S))) {
			player.walkingstate = SITTING;
		}
		
		/* remove/place blocks */
		if (qw_mousedown(QW_MOUSE_LEFT)) {
			int x, y;
			grid_world_to_grid(grid, qw_mousex, qw_mousey, &x, &y);
			
			block_t *current = grid_at(grid, x, y);
			current->damage += 1;
			if (current->damage >= current->damage_max) {
				current->id = -1;
			}

			if (current->id == -1) {
				if (player.arm_build_timer == 0)
					player.arm_build_timer = 1;
			} else {
				if (player.arm_build_timer == 0)
					player.arm_build_timer = 10;
			}
		} else if (qw_mousedown(QW_MOUSE_RIGHT)) {
			int x, y;
			grid_world_to_grid(grid, qw_mousex, qw_mousey, &x, &y);

			block_t *current = grid_at(grid, x, y);
			if (current->id < 0) {
				*current = block_new(player.selected_blockid);
				
				if (player.arm_build_timer == 0) {
					player.arm_build_timer = 10;
				}
				
				/* check if block below is grass -place-> dirt */
				block_t *below = grid_at(grid, x, y + 1);
				if (below->id == 0) {
					below->id = 1;
				}
			}
		}
		
		/* block selection */
		if (qw_keydown(QW_KEY(1))) player.selected_blockid = 1 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(2))) player.selected_blockid = 2 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(3))) player.selected_blockid = 3 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(4))) player.selected_blockid = 4 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(5))) player.selected_blockid = 5 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(6))) player.selected_blockid = 6 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(7))) player.selected_blockid = 7 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(8))) player.selected_blockid = 8 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(9))) player.selected_blockid = 9 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
		if (qw_keydown(QW_KEY(0))) player.selected_blockid = 0 + (qw_keydown(QW_KEY(LSHIFT)) ? 9 : 0) + (qw_keydown(QW_KEY(LCTRL)) ? 9 : 0);
	

		/* update player */
		enemy_update(&zombie0, &grid);
		player_update(&player, &grid);
		if (player.jumpstate == ONGROUND && player.jumpstate_last == FALLING && player.lastvel.y == player.maxvy) {
			for (particles_count = 0; particles_count < 32; ++particles_count) {
				particles[particles_count] = particle_new(vector_sub(player.pos, vector_new(0, 3.5)), vector_new((rand() % 20 - 10) / 2.0, (rand() % 10 - 10) / 2.0), &particle_blood);
			}
		}

		enemy_updatelast(&zombie0, &grid);
		player_updatelast(&player, &grid);
		grid.xoff = -player.pos.x + qw_width / 2.0;
		grid.yoff = -player.pos.y + 1.75 * (qw_height / 3.0);

		/* render all blocks on screen, player, ... */
		grid_draw(grid, (-grid.xoff) / grid.blocksize, 0, (qw_width / grid.blocksize) + grid.blocksize, grid.height);
		enemy_draw(&zombie0, grid);
		player_draw(&player, grid);
		
		/* render particles */
		for (int i = 0; i < particles_count; ++i) {
			particle_update(&particles[i], &grid);
			particle_draw(&particles[i], &grid);
		}

		DEBUG_BEGIN
			/* currently selected block*/
			qw_color(255, 255, 0, 255);
			qw_write("Block: ", 20, 20);
			block_draw(grid.spritesheet, 0, 0, block_new(player.selected_blockid), 75, 16, grid.blocksize / 2);
			/* player states */
			char pwalkingstate[32] = {0};
			char pjumpingstate[32] = {0};
			char pdaytext[16] = {0};
			sprintf(pwalkingstate, "Walking: %s", (player.walkingstate == STANDING ? "Standing" : (player.walkingstate == WALKING ? "Walking" : "Sitting")));
			sprintf(pjumpingstate, "Jumping: %s", (player.jumpstate == JUMPING ? "Jumping" : (player.jumpstate == FALLING ? "Falling" : "On ground")));
			sprintf(pdaytext, "Day: %hd", days);
			qw_write(pwalkingstate, 20, 40);
			qw_write(pjumpingstate, 20, 60);
			qw_write(pdaytext, 20, 80);
			/* player position */
			char ppostext[32] = {0};
			char pveltext[32] = {0};
			sprintf(ppostext, "Pos: %.4g, %.4g", player.pos.x, player.pos.y);
			sprintf(pveltext, "Vel: %.4g, %.4g", player.vel.x, player.vel.y);
			qw_write("Player:", 20, 100);
			qw_write(ppostext, 30, 110);
			qw_write(pveltext, 30, 120);
		DEBUG_END

		/* day/night cycle */
		if (sunrise)
			++daytime;
		else
			--daytime;

		if (daytime >= daylength) {
			if (sunrise == 1) {
				++days;
				sunrise = 0;
			}
		} else if (daytime <= 0) {
			if (sunrise == 0) {
				++days;
				sunrise = 1;
			}
		}
		
		qw_redraw();
		/* slow motion, debug stuff */
		if (qw_keydown(QW_KEY(SPACE)))
			qw_delay(75);
	}
	
	grid_saveto(&grid, "world0.sav");

	particle_preset_delete(&particle_blood);

	player_delete(&player);
	grid_delete(&grid);
	qw_quit();
	return 0;
}
