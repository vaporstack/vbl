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

typedef void (*vbl_particlesystem_draw_fun)(void*);
typedef void (*vbl_particlesystem_draw_particle_fun)(void*, VParticle*);

typedef struct VParticleSystem
{
	VParticle*   data;
	unsigned int num;
	void**       plugins;
	unsigned int num_plugins;
	vbl_particlesystem_draw_particle_fun draw_particle;
	vbl_particlesystem_draw_fun draw;
} VParticleSystem;


typedef struct VParticleSystemHnd
{
	VParticleSystem* src;
} VParticleSystemHnd;



void vbl_particlesystem_draw(VParticleSystem* sys);

VParticleSystem* vbl_particlesystem_create(void);
VParticleSystem* vbl_particlesystem_create_with_defaults(void);
void		 vbl_particlesystem_destroy(VParticleSystem* sys);

void vbl_particlesystem_plugin_add(VParticleSystem* sys, VParticlePlugin* plug);

void vbl_particlesystem_update(VParticleSystem* sys);
void vbl_particlesystem_particle_spawn(VParticleSystem* sys);

#endif /* g_particle_system2_h */
