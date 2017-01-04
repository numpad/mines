#ifndef PARTICLE_H
#define PARTICLE_H

#include "rgb.h"
#include "vector.h"

/* some preset particle behavior */
typedef enum {
	PARTICLE_NORMAL, PARTICLE_FLUID
} particle_type;

/* some death behavior */
typedef enum {
	PARTICLEDEATH_NORMAL, PARTICLEDEATH_FADEOUT
} particle_death_type;

/* particle presets */
typedef struct {
	rgb_t *colors;
	size_t _color_i;

	unsigned int lifetime_min, lifetime_max;
	unsigned int size_min, size_max;

	particle_type type;
	particle_death_type deathtype;
} particle_preset_t;

/* delete a particle preset */
void particle_preset_delete(particle_preset_t *pp) {
	free(pp->colors);
}

/* returns a new particle preset */
particle_preset_t particle_preset_new(size_t colors, unsigned int lifetime_min, unsigned int lifetime_max, unsigned int size_min, unsigned int size_max) {
	particle_preset_t pp;
	
	pp.colors = malloc(colors * sizeof(rgb_t));
	/* lifetime */
	pp.lifetime_min = lifetime_min;
	pp.lifetime_max = lifetime_max;
	/* size */
	pp.size_min = size_min;
	pp.size_max = size_max;
	
	pp.type = PARTICLE_NORMAL;
	pp.deathtype = PARTICLEDEATH_NORMAL;

	pp._color_i = 0;

	return pp;
}

/* adds a color to the particle preset */
void particle_preset_addcolor(particle_preset_t *pp, rgb_t color) {
	pp->colors[pp->_color_i++] = color;
}

typedef struct {
	vector_t pos, vel;
	unsigned int lifetime,
	             ticks;
	float sizex, sizey;
	
	char collides_blocks;
	char dead;

	particle_type type;
	particle_death_type deathtype;

	rgb_t color;
	unsigned char alpha;
} particle_t;

particle_t particle_new(vector_t pos, vector_t vel, particle_preset_t *pp) {
	particle_t p;
	
	p.ticks = 0;
	p.dead = 0;

	/* position, velocity set */
	p.pos = pos;
	p.vel = vel;
	/* lifetime, size generation */
	p.lifetime = rand() % (pp->lifetime_max - pp->lifetime_min) + pp->lifetime_min;
	const int size = rand() % (pp->size_max - pp->size_min) + pp->size_min;
	p.sizex = size;
	p.sizey = size;
	/* random color */
	p.color = pp->colors[rand() % pp->_color_i];
	p.alpha = 255;

	/* collision groups */
	p.collides_blocks = 1;
	/* type preset */
	p.type = pp->type;
	p.deathtype = pp->deathtype;

	return p;
}

void particle_update(particle_t *p, grid_t *grid) {
	if (p->ticks > p->lifetime) {
		if (p->deathtype == PARTICLEDEATH_NORMAL) {
			p->dead = 1;
		} else if (p->deathtype == PARTICLEDEATH_FADEOUT) {
			if (p->alpha > 0) {
				p->alpha *= 0.9;
			} else {
				p->dead = 1;
			}
		}
		return;
	}
	p->ticks++;
	
	vector_t tmpvel = p->vel;
	
	if (p->collides_blocks) {
		/* current block touched by particle */
		int bx, by;
		/* block touched by particle after one position update */
		int nextbx, nextby;
		grid_world_to_grid(*grid, p->pos.x + grid->xoff, p->pos.y + grid->yoff, &bx, &by);
		grid_world_to_grid(*grid, p->pos.x + p->vel.x + grid->xoff, p->pos.y + p->vel.y + grid->yoff, &nextbx, &nextby);
		
		int collides_current = block_collides(grid_getblock(*grid, bx, by)),
			collides_next    = block_collides(grid_getblock(*grid, nextbx, nextby));

		if (collides_current) {
			p->vel.x = 0;
			p->vel.y = 0;
			if (p->type == PARTICLE_FLUID) {
				const float sizediff = p->sizey / p->sizex;
				if (sizediff > 0.34) {
					p->sizex += 1.0/sizediff;
				}
			}
		} else if (!collides_current && collides_next) {
			p->vel.x = 0;
			p->vel.y = 0;
		} else {
			p->vel.y += 0.35;
			p->pos = vector_add(p->pos, p->vel);
		}
	} else {
		p->vel.y += 0.35;
		p->pos = vector_add(p->pos, p->vel);
	}

}

void particle_draw(particle_t *p, grid_t *grid) {
	if (!p->dead) {
		qw_color(p->color.r, p->color.g, p->color.b, p->alpha);
		qw_fillrect(p->pos.x + grid->xoff - p->sizex / 2.0, p->pos.y + grid->yoff - p->sizey / 2.0, p->sizex, p->sizey);
	}
}

#endif
