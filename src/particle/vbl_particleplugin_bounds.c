
//
//  vbl_particleplugin_bounds.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_bounds.h"

#include "vbl_particle.h"
#include "vbl_particlesystem.h"
#include <stdlib.h>
#include "../core/vbl_log.h"

#include "vbl_particleplugin_emitter.h"
static void destroy(void* idk1, void* idk2)
{
}

static void bounds_adjust_kill(void* data, void* idk)
{
}

static void bounds_adjust_reset(void* plugdata, void* sysdata, void* pdata)
{
	VParticleSystem* sys = sysdata;
	VParticle* p = pdata;
	
	VParticlePlugin* emitter = sys->emitter;
	if ( !emitter )
	{
		vbl_log("Nothing we can do here.");
		return;
	}

	vbl_particleplugin_emitter_reset(emitter, sys, p);

}

static void bounds_adjust_bounce(void* data, void* idk)
{
}

#include <stdbool.h>

static int bounds_constrain_floor(void* data, void* pdata)
{
	return false;
}


VPPSBoundsInfo* vbl_particleplugin_boundsinfo_create(void)
{
	VPPSBoundsInfo* info = calloc(1, sizeof(VPPSBoundsInfo));
	
	return info;
}

static int bounds_constrain_box(void* data, void* pdata)
{
	VParticle*       p    = pdata;
	VParticlePlugin* plug = data;
	VPPSBoundsInfo*  inf  = plug->data;
	VParticle	old  = *p;
	
	bool crossed_x = false;
	bool crossed_y = false;
	bool crossed_z = false;
	if (p->x < -inf->bnd.x || p->x >inf->bnd.x)
	{
		double v = inf->bnd.x;
		if (p->x < 0)
			v *= -1;
		p->x = v;

		crossed_x = true;
	}
	if (p->y < -inf->bnd.y || p->y >inf->bnd.y)
	{
		double v = inf->bnd.y;
		if (p->y < 0)
			v *= -1;
		p->y = v;
		
		crossed_y = true;
	}
	
	if (p->z < -inf->bnd.z || p->z >inf->bnd.z)
	{
		double v = inf->bnd.z;
		if (p->z < 0)
			v *= -1;
		p->z = v;
		
		crossed_z = true;
	}


	switch (inf->bounds_behavior) {
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL:
			
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_BOUNCE:
			if ( crossed_x)
				p->vx *= -inf->bounce_drag;
			if ( crossed_y)
				p->vy *= -inf->bounce_drag;
			if ( crossed_z)
				p->vz *= -inf->bounce_drag;
			break;
		default:
			break;
	}


	//if (p->x < -inf->bnd.x)
	//	p->x = -inf->bnd.x;
	//if (p->x > inf->bnd.x)
	//	p->x = inf->bnd.x;
//	if (p->y < -inf->bnd.y)
//		p->y = -inf->bnd.y;
//	if (p->y > inf->bnd.y)
//		p->y = inf->bnd.y;
//	if (p->z < -inf->bnd.z)
//		p->z = -inf->bnd.z;
//	if (p->z > inf->bnd.z)
//		p->z = inf->bnd.z;

	return (old.x != p->x || old.y != p->y || old.z != p->z);

	//printf("asdf\n");
}

static int bounds_constrain_sphere(void* data, void* pdata)
{
	VParticlePlugin* plug = data;
	VPPSBoundsInfo*  info  = plug->data;
	VParticle* p = pdata;
	vec3_t a = vec3_create(NULL);
	vec3_t b = vec3_create(NULL);
	
	a[0] = info->pos.x;
	a[1] = info->pos.y;
	a[2] = info->pos.z;
	b[0] = p->x;
	b[1] = p->y;
	b[2] = p->z;
	double d = vec3_dist(a, b);
	
	free(a);
	free(b);
	
	
	double r = info->radius;
	switch (info->bounds_volume) {
		case VBL_PARTICLE_VOLUMETYPE_NONE:
			return false;
			
		case VBL_PARTICLE_VOLUMETYPE_WITHIN:
			return ( d < r );
		
		case VBL_PARTICLE_VOLUMETYPE_SURFACE:
			return ( d == r );
		
		case VBL_PARTICLE_VOLUMETYPE_OUTSIDE:
			return ( d > r );
			
		default:
			break;
	}
	
	printf("Failed all boundary test cases!\n");
	return false;
}

#ifdef DEBUG
#include <drw/drw.h>

static void draw_debug_box(VPPSBoundsInfo* info)
{
}

void draw_debug(VParticlePlugin* plug)
{
	VPPSBoundsInfo* info = plug->data;
	drw_cube(info->bnd.x * 2);

	//drw_push();
	//drw_translate(info->)
}

#endif

static void update(void* plugd, void* sysd)
{
	VParticleSystem* sys  = sysd;
	VParticlePlugin* plug = plugd;
	VPPSBoundsInfo*  info = plug->data;

	for (unsigned i = 0, n = sys->max; i < n; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;

		int violated = false;
		if (info->constrain_func)
			violated = info->constrain_func(plugd, p);

		if (!violated)
			continue;

		switch (info->bounds_behavior)
		{
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL:
			free(p);
			sys->data[i] = NULL;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_RESET:
				info->adjust_func(plug, sys, p);
			break;

		default:
			break;
		}
		//		if (info->adjust_func)
		//			info->adjust_func(plugd, p);
	}
}

VParticlePlugin* vbl_particleplugin_bounds_create(VPPSBoundsInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();

	//VPPSBoundsFRec* rec = calloc(1, sizeof(VPPSBoundsFRec));
	info->constrain_func = NULL;
	info->adjust_func    = NULL;

	switch (info->bounds_type)
	{
	case VBL_PARTICLEPLUGIN_BOUNDSTYPE_PLANE:
		info->constrain_func = bounds_constrain_floor;
		break;
	case VBL_PARTICLEPLUGIN_BOUNDSTYPE_BOX:
		info->constrain_func = bounds_constrain_box;
		break;
	case VBL_PARTICLEPLUGIN_BOUNDSTYPE_SPHERE:
		info->constrain_func = bounds_constrain_sphere;
		if ( info->radius == 0)
		{
			vbl_log("alert, created a sphere bounds with size of zero! setting to default (1)");
			info->radius = 1;
		}
		break;
	default:
		break;
	}

	switch (info->bounds_behavior)
	{
	case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL:
		info->adjust_func = bounds_adjust_kill;
		break;
	case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_BOUNCE:
		info->adjust_func = bounds_adjust_bounce;
		break;
	case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_RESET:
		info->adjust_func = bounds_adjust_reset;
		break;

	default:
		break;
	}

	info->bounce_drag = .44;
	//free(rec);
	plug->data	= info;
	plug->destroyself = destroy;
	plug->update      = update;
#ifdef DEBUG
	plug->draw_debug = draw_debug;
#endif
	return plug;
}
