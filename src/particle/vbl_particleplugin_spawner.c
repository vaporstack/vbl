//
//  vbl_particleplugin_spawner.c
//  vbl
//
//  Created by vs on 12/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_spawner.h"

#include <stdlib.h>


//	TODO: untangle this
#include "../core/vbl_rng.h"

#include <r4/r4.h>

static void place_point(VParticlePlugin* plug,VParticleSystem * sys, double* x, double *y, double* z)
{
	
}
static void place_box(VParticlePlugin* plug,VParticleSystem * sys, double* x, double *y, double* z)
{
	VPPSSpawnerInfo* info = plug->data;

	RRandom* rng = vbl_rng_get();
	
	*x = info->bx * -.5 + r_rand_double(rng) * info->bx;
	*y = info->by * -.5 + r_rand_double(rng) * info->by;
	*z = info->bz * -.5 + r_rand_double(rng) * info->bz;
}

static void place_sphere(VParticlePlugin* plug,VParticleSystem * sys, double* x, double *y, double* z)
{
}

static unsigned density_one(struct VPPSSpawnerInfo* info)
{
	return 1;
}

static unsigned density_fixed(struct VPPSSpawnerInfo* info)
{
	return info->density_n;
}

static bool check_frame(struct VPPSSpawnerInfo* info)
{
	return true;
}
static bool check_time(struct VPPSSpawnerInfo* info)
{
	return true;
}

#include "../core/vbl_rng.h"

static void update(void* dplug, void* dsys)
{
	VParticlePlugin* plug = dplug;
	VParticleSystem* sys = dsys;
	
	VPPSSpawnerInfo* info = plug->data;
	
	if ( !info->check(info) )
		return;
	
	unsigned num = info->density(info);
	
	if ( num == 0 )
		return;
	
	signed (*next_fun)(VParticleSystem*);
	next_fun =  ( info->reuse ) ? vbl_particlesystem_next : vbl_particlesystem_next_available;
	RRandom* rng = vbl_rng_get();
	for ( unsigned int i = 0; i < num; i++ )
	{
		unsigned id = next_fun(sys);
		if ( id == -1 )
			continue;
		
		VParticle* p = sys->data[id];
		if ( p )
		{
			printf("Something went WRONG\n");
			continue;
		}
		
		p = vbl_particle_create();
		p->mass = r_rand_double(rng) * 50;
		if ( p->mass == 0 )
		{
			p->mass = .00001;
			printf("Correct mass to %f\n", p->mass);
		}
		
		info->place(plug, sys, &p->x, &p->y, &p->z );
		sys->data[id] = p;
		
		printf("Placed new particle at %.1f %.1f %.1f\n", p->x, p->y, p->z);
		
	}
	
	
}

static void setup(VParticlePlugin* plug)
{
	VPPSSpawnerInfo* info = plug->data;
	
	switch (info->spawn_type) {
		case VBL_PARTICLEPLUGIN_SPAWNERTYPE_POINT:
			info->place =  place_point;
			break;
		case VBL_PARTICLEPLUGIN_SPAWNERTYPE_BOX:
			info->place =  place_box;
			break;
		case VBL_PARTICLEPLUGIN_SPAWNERTYPE_SPHERE:
			info->place = place_sphere;
		default:
			break;
	}
	
	switch (info->spawn_freq) {
		case VBL_PARTICLEPLUGIN_SPAWNERFREQ_FRAME:
			info->check = check_frame;
			break;
		case VBL_PARTICLEPLUGIN_SPAWNERFREQ_TIME:
			info->check = check_time;
		default:
			break;
	}
	switch (info->spawn_density) {
		case VBL_PARTICLEPLUGIN_SPAWNERDENSITY_ONE:
			info->density = density_one;
			break;
		case VBL_PARTICLEPLUGIN_SPAWNERDENSITY_FIXED:
			info->density = density_fixed;
			break;
			
		default:
			break;
	}
}

VPPSSpawnerInfo* vbl_particleplugin_spawnerinfo_create(void)
{
	VPPSSpawnerInfo* info = calloc(1, sizeof(VPPSSpawnerInfo));
	info->spawn_density = VBL_PARTICLEPLUGIN_SPAWNERDENSITY_ONE;
	info->spawn_freq = VBL_PARTICLEPLUGIN_SPAWNERFREQ_TIME;
	info->spawn_type = VBL_PARTICLEPLUGIN_SPAWNERTYPE_POINT;
	info->density_n = 3;
	
	return info;
}

VParticlePlugin* vbl_particleplugin_spawner_create(VPPSSpawnerInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	plug->data = info;
	plug->update = update;
	setup(plug);
	
	return plug;
}
