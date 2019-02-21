//
//  vbl_particleplugin_wind.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_wind.h"
#include "vbl_particlesystem.h"

//	todo: transition to a plugin updating an individual particle so we
//	can eventually do them all in parallel?
#include <vbl/src/core/vbl_rng.h>

#include <stdlib.h>

static void update_one(VParticlePlugin* plug, VParticle* p)
{
	VPPSWindInfo* info = plug->data;

	p->ax += info->x;
	p->ay += info->y;
	p->az += info->z;
}

#include <r4/src/core/r_math.h>
#include <r4/src/core/r_random.h>
/*
static inline double clamp(double value, double min, double max)
{
	return value < min ? min : value > max ? max : value;
}

static inline double nclamp(double value)
{
	return value < 0 ? 0 : value > 1 ? 1 : value;
}
*/
static void update_variance(VPPSWindInfo* info)
{
	RRandom* rng = vbl_rng_get();

	double vx = (r_rand_double(rng) * info->vspeed) - info->vspeed * .5;
	double vy = (r_rand_double(rng) * info->vspeed) - info->vspeed * .5;
	double vz = (r_rand_double(rng) * info->vspeed) - info->vspeed * .5;

	info->vx += vx;
	info->vy += vy;
	info->vz += vz;
	info->vx = clamp(info->vx, -info->vspeed, info->vspeed);
	info->vy = clamp(info->vy, -info->vspeed, info->vspeed);
	info->vz = clamp(info->vz, -info->vspeed, info->vspeed);

	info->x += info->vx;
	info->y += info->vy;
	info->z += info->vz;

	info->x = clamp(info->x, -info->vmx, info->vmx);
	info->y = clamp(info->y, -info->vmy, info->vmy);
	info->z = clamp(info->z, -info->vmz, info->vmz);
	//	info->vx = clamp
}

static void update(void* pdata, void* sdata)
{
	VParticlePlugin* plug = pdata;
	VParticleSystem* sys  = sdata;
	VPPSWindInfo*    info = plug->data;

	if (info->variance)
		update_variance(info);

	for (unsigned i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;
		double tx = (1. / p->mass);
		tx *= info->x;

		p->ax += (1. / p->mass) * info->x;
		p->ay += (1. / p->mass) * info->y;
		p->az += (1. / p->mass) * info->z;
	}
}

#ifdef DEBUG
#include <drw/drw.h>

static void draw_debug(VParticlePlugin* plug, void* idk)
{
	VPPSWindInfo* i = plug->data;
	drw_line_3f(0, 0, 0, .5 * i->x, 0, 0);
	drw_line_3f(0, 0, 0, 0, .5 * i->y, 0);
	drw_line_3f(0, 0, 0, 0, 0, .5 * i->z);

	char* buf = calloc(64, sizeof(char));
	sprintf(buf, "%.2f %.2f %.2f", i->x, i->y, i->z);

	//drw_push();

	//drw_type_draw(buf);
	free(buf);
}
#endif

VParticlePlugin* vbl_particleplugin_wind_createdefault(int varying, double vx, double vy, double vz)
{
	VPPSWindInfo* winfo = calloc(1, sizeof(VPPSWindInfo));
	winfo->variance     = true;
	winfo->vspeed       = 1 - .00000001;
	winfo->vmx = winfo->vmy = winfo->vmz = .0001;
	VParticlePlugin* wind = vbl_particleplugin_wind_create(winfo);
	return wind;
	//vbl_particlesystem_mgr_plugin_subplugin_register(particle_mgr, wind);
}

VParticlePlugin* vbl_particleplugin_wind_create(VPPSWindInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();

	plug->data   = info;
	plug->update = update;
#ifdef DEBUG
	plug->draw_debug = draw_debug;
#endif
	return plug;
}
