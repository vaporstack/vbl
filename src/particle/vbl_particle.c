//
//  vbl_particle.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particle.h"

#include <stdlib.h>

VParticle* vbl_particle_create(void)
{
	VParticle* p = calloc(1, sizeof(VParticle));
	
	return p;
}

void vbl_particle_destroy(VParticle* p)
{
	free(p);
}

static inline double clamp(double value, double min, double max)
{
	return value < min ? min : value > max ? max : value;
}

#define TMP_MAX .0001
void vbl_particle_update(VParticle* p)
{
	//	add acceleration to velocity
	p->vx += p->ax;
	p->vy += p->ay;
	p->vz += p->az;

	//	clear acceleration
	p->ax = p->ay = p->az = 0;

	//	clamp velocity
	p->vx = clamp(p->vx, -TMP_MAX, TMP_MAX);
	p->vy = clamp(p->vy, -TMP_MAX, TMP_MAX);
	p->vz = clamp(p->vz, -TMP_MAX, TMP_MAX);
	//	add velocity to position
	p->x += p->vx;
	p->y += p->vy;
	p->z += p->vz;
}
