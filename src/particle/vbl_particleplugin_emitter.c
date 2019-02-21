//
//  vbl_particleplugin_emitter.c
//  vbl
//
//  Created by vs on 12/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_emitter.h"

#include <stdlib.h>

//	TODO: untangle this
#include "../core/vbl_rng.h"

#include <r4/r4.h>
#include "../geo/v_primitives.h"

static void set_velocity(VParticlePlugin* plug, VParticleSystem* sys, VParticle* p)
{
	VPPSEmitInfo* info = plug->data;

	switch (info->emit_dir)
	{
	case VBL_PARTICLEPLUGIN_EMITDIR_NONE:
		p->vx = p->vy = p->vz = 0;
		break;
	case VBL_PARTICLEPLUGIN_EMITDIR_CONE:
		break;

	case VBL_PARTICLEPLUGIN_EMITDIR_DIRECTIONAL:
		p->ax += info->vx;
		p->ay += info->vy;
		p->az += info->vz;
		break;

	default:
		break;
	}
}

static void place_point(VParticlePlugin* plug, VParticleSystem* sys, double* x, double* y, double* z)
{
	VPPSEmitInfo* info = plug->data;
	*x = info->ox;
	*y = info->oy;
	*z = info->oz;
}

static void place_box(VParticlePlugin* plug, VParticleSystem* sys, double* x, double* y, double* z)
{
	VPPSEmitInfo* info = plug->data;

	RRandom* rng = vbl_rng_get();
	RPoint3 p;
	
	switch (info->spawn_volume) {
		case VBL_PARTICLE_VOLUMETYPE_WITHIN:
			p = v_primitives_random_point_in_box(info->bx, info->by, info->bz);
			break;
		case VBL_PARTICLE_VOLUMETYPE_SURFACE:
			p = v_primitives_random_point_on_box(info->bx, info->by, info->bz);
			break;
			
		default:
			break;
	}
	*x = info->ox + (info->bx * -.5 + r_rand_double(rng) * info->bx);
	*y = info->oy + (info->by * -.5 + r_rand_double(rng) * info->by);
	*z = info->oz + (info->bz * -.5 + r_rand_double(rng) * info->bz);
}


static void place_sphere(VParticlePlugin* plug, VParticleSystem* sys, double* x, double* y, double* z)
{
	VPPSEmitInfo* info = plug->data;
	double	r    = info->r * (1. / 10.);
	//RPoint3 p = v_primitives_random_point_on_sphere(r);
	//RPoint3 p = v_primitives_random_point_in_sphere(r);
	RPoint3 p;
	
	switch (info->spawn_volume) {
		case VBL_PARTICLE_VOLUMETYPE_WITHIN:
			p = v_primitives_random_point_in_sphere(r);
			break;
		case VBL_PARTICLE_VOLUMETYPE_SURFACE:
			p = v_primitives_random_point_on_sphere(r);
			break;
		default:
			break;
	}
	*x	= p.x;
	*y	= p.y;
	*z	= p.z;
	
	//RPoint3 p = v_primitives_random_point_on_sphere(1);
	//RPoint3 p = v_primitives_random_point_on_box(1,1,1);
}

static unsigned density_one(struct VPPSEmitInfo* info)
{
	return 1;
}

static unsigned density_fixed(struct VPPSEmitInfo* info)
{
	return info->density_n;
}

static void trigger_event(void* sys)
{

	printf("YAHAHA emitted\n");
}

static bool check_frame(struct VPPSEmitInfo* info)
{
	return true;
}
#include <r4/src/core/r_time.h>

static bool check_time(struct VPPSEmitInfo* info)
{

	double now   = r_time();
	double delta = now - info->last;
	if (delta > info->frequency)
	{
		//printf("poop\n");
		info->last = now;
		return true;
	}
	return false;
}

#include "../core/vbl_rng.h"


void		vbl_particleplugin_emitter_reset(VParticlePlugin* plug, VParticleSystem* sys, VParticle* p)
{
	VPPSEmitInfo* info = plug->data;
	info->place(plug, sys, &p->x, &p->y, &p->z);
	set_velocity(plug, sys, p);

}


static void update(void* dplug, void* dsys)
{
	VParticlePlugin* plug = dplug;
	VParticleSystem* sys  = dsys;

	VPPSEmitInfo* info = plug->data;

	if (!info->check(info))
		return;

	unsigned num = info->density(info);

	if (num == 0)
		return;

	signed (*next_fun)(VParticleSystem*);
	next_fun     = (info->reuse) ? vbl_particlesystem_next : vbl_particlesystem_next_available;
	RRandom* rng = vbl_rng_get();
	for (unsigned int i = 0; i < num; i++)
	{
		unsigned id = next_fun(sys);
		if (id == -1)
			continue;

		VParticle* p = sys->data[id];
		if (p)
		{
			printf("Something went WRONG\n");
			continue;
		}

		p       = vbl_particle_create();
		p->mass = r_rand_double(rng) * 50;
		if (p->mass == 0)
		{
			p->mass = .00001;
			printf("Correct mass to %f\n", p->mass);
		}

		info->place(plug, sys, &p->x, &p->y, &p->z);
		set_velocity(plug, sys, p);

		sys->data[id] = p;

		printf("Placed new particle at %.1f %.1f %.1f\n", p->x, p->y, p->z);
	}
}

static void setup(VParticlePlugin* plug)
{
	VPPSEmitInfo* info = plug->data;

	switch (info->spawn_type)
	{
	case VBL_PARTICLEPLUGIN_EMITTYPE_POINT:
		info->place = place_point;
		break;
	case VBL_PARTICLEPLUGIN_EMITTYPE_BOX:
		info->place = place_box;
		break;
	case VBL_PARTICLEPLUGIN_EMITTYPE_SPHERE:
		info->place = place_sphere;
	default:
		break;
	}

	switch (info->spawn_freq)
	{
	case VBL_PARTICLEPLUGIN_EMITFREQ_FRAME:
		info->check = check_frame;
		break;
	case VBL_PARTICLEPLUGIN_EMITFREQ_TIME:
		info->check = check_time;
		///info->frequency = 1;
		info->last = 0;
	default:
		break;
	}
	switch (info->spawn_density)
	{
	case VBL_PARTICLEPLUGIN_EMITDENSITY_ONE:
		info->density = density_one;
		break;
	case VBL_PARTICLEPLUGIN_EMITDENSITY_FIXED:
		info->density = density_fixed;
		break;

	default:
		break;
	}
}

VPPSEmitInfo* vbl_particleplugin_emitterinfo_create(void)
{
	VPPSEmitInfo* info  = calloc(1, sizeof(VPPSEmitInfo));
	info->spawn_density = VBL_PARTICLEPLUGIN_EMITDENSITY_ONE;
	info->spawn_freq    = VBL_PARTICLEPLUGIN_EMITFREQ_TIME;
	info->spawn_type    = VBL_PARTICLEPLUGIN_EMITTYPE_POINT;
	info->density_n     = 1;
	info->trigger       = trigger_event;
	return info;
}

VParticlePlugin* vbl_particleplugin_emitter_create(VPPSEmitInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	plug->type = VBL_PARTICLEPLUGIN_TYPE_EMITTER;
	plug->data	    = info;
	plug->update	  = update;
	setup(plug);

	return plug;
}

VParticlePlugin* vbl_particleplugin_emitter_create_point(void)
{
	VPPSEmitInfo* info = vbl_particleplugin_emitterinfo_create();
	info->spawn_density = VBL_PARTICLEPLUGIN_EMITDENSITY_ONE;
	info->spawn_freq = VBL_PARTICLEPLUGIN_EMITFREQ_FRAME;
	info->spawn_type = VBL_PARTICLEPLUGIN_EMITTYPE_POINT;
	//info->density = 1;
	//info->place = place_point;
	
	VParticlePlugin* plug = vbl_particleplugin_emitter_create(info);
	setup(plug);
	return plug;
}

VParticlePlugin* vbl_particleplugin_emitter_create_box(double x, double y, double z)
{
	//that should get  our attention later
	return NULL;
}
