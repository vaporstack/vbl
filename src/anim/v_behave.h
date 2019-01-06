
//  v_behave.c
//  v_behave
//
//  Created by Andrew Macfarlane on 11/20/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_behave_h_
#define v_behave_h_

#include <grdn/src/particle/g_particle_system.h>

typedef void (*move_func)(GParticleSystem* sys, Particle* p);
typedef int (*test_func)(GParticleSystem* sys, Particle* p, double dist);

#include <vector/vector.h>

typedef struct VBehave
{
	// struct vector_t behaviors;
	move_func move;
	test_func test;
	move_func start;
	move_func end;
	double    distance;
} VBehave;

VBehave* v_behave_create(void);
void     v_behave_destroy(VBehave* rec);

#endif
