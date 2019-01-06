
//  v_swarm.c
//  v_swarm
//
//  Created by Andrew Macfarlane on 11/20/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_swarm_h_
#define v_swarm_h_

#include "v_behave.h"
#include <grdn/grdn.h>
#include <r4/src/geo/r_object.h>

//	pair particle systems with behaviors to start making some
//	interesting shit
#define VSWARM_MAX_BEHAVIORS 4
#define SWARM_MAX 128
#define TRAIL_LENGTH 512



typedef struct VSwarm
{
	int		 num;
	int		 which;
	VBehave*	 behave[VSWARM_MAX_BEHAVIORS];
	VBehave*	 current;
	VBehave*	 next;
	GParticleSystem* sys;
	RLine3		 trails[SWARM_MAX];
} VSwarm;

typedef void (*swarm_draw_fun)(VSwarm* s);

void v_swarm_set_draw_func(swarm_draw_fun fun);

VSwarm* v_swarm_create(int num);
void    v_swarm_destroy(VSwarm* swarm);
void    v_swarm_update(VSwarm* swarm);
void    v_swarm_draw(VSwarm* swarm);

#endif
