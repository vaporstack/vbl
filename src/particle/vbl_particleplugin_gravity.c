//
//  vbl_particleplugin_gravity.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_gravity.h"

#include "vbl_particlesystem.h"
#include <stdlib.h>

#ifdef ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE
//#error hi
#include <drw/drw.h>

#endif

typedef struct GravityRec
{
	double x, y, z;

} GravityRec;

static void apply_gravity(void* pdata, void* sdata)
{
	GravityRec*      grv = (GravityRec*)pdata;
	VParticleSystem* sys = (VParticleSystem*)sdata;
	if ( !sys->data)
		return;
	for (unsigned int i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;
		p->ax += grv->x;
		p->ay += grv->y;
		p->az += grv->z;
	}
}
#ifdef DEBUG
//typedef void (*vbl_particle_plugin_fun)(void* data, void* plugdata);
static void draw_debug(void* pdata, void* sdata)
{
	VParticlePlugin* plug = pdata;
	VParticleSystem* sys  = sdata;
	GravityRec* info = plug->data;
	
	drw_push();
	//drw_translate(-.125, -.125, 0);
	drw_axis_living();
	drw_line_3f(0,0,0, info->x, info->y, info->z);
	
	drw_translate(info->x, info->y, info->z);

	drw_type_draw("%.2f %.2f %.2f", info->x, info->y, info->z );
	drw_axis();
	drw_pop();
	
}

#endif

VParticlePlugin* vbl_particleplugin_gravity_create(double x, double y, double z)
{

	VParticlePlugin* plug = vbl_particleplugin_create();
	plug->update	  = apply_gravity;
	GravityRec* grv      = calloc(1, sizeof(GravityRec));
	grv->x		     = x;
	grv->y		     = y;
	grv->z		     = z;
	plug->data	    = grv;
#ifdef DEBUG
	plug->draw_debug = draw_debug;
#endif
	
	return plug;
}
