//
//  vbl_particlesystem2.h
//  vbl
//
//  Created by vs on 11/24/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#ifndef g_particle_system2_h
#define g_particle_system2_h

#include "vbl_particle.h"
#include "vbl_particleplugin.h"
#include <stdio.h>

#ifdef ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE
typedef void (*vbl_particlesystem_draw_fun)(void*);
typedef void (*vbl_particlesystem_draw_particle_fun)(void*, VParticle*);
#endif

typedef struct VParticleSystem
{
	VParticle**  data;
	unsigned int max;
	void**       plugins;
	unsigned int num_plugins;
	unsigned int pos;
#ifdef ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE
#ifdef DEBUG
	vbl_particlesystem_draw_fun draw_debug;
#endif
	vbl_particlesystem_draw_particle_fun draw_particle;
	vbl_particlesystem_draw_fun	  draw;
#endif

} VParticleSystem;

typedef struct VParticleSystemHnd
{
	VParticleSystem* src;
} VParticleSystemHnd;

#ifdef ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE
void vbl_particlesystem_draw(VParticleSystem* sys);
#endif

VParticleSystem* vbl_particlesystem_create(unsigned);
VParticleSystem* vbl_particlesystem_create_with_defaults(unsigned);
void		 vbl_particlesystem_destroy(VParticleSystem* sys);

void vbl_particlesystem_plugin_add(VParticleSystem* sys, VParticlePlugin* plug);

void vbl_particlesystem_update(VParticleSystem* sys);
//void vbl_particlesystem_particle_spawn(VParticleSystem* sys);

signed vbl_particlesystem_next_available(VParticleSystem* sys);
signed vbl_particlesystem_next(VParticleSystem* sys);

#endif /* g_particle_system2_h */
