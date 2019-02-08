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

static void destroy(void* idk1, void* idk2)
{
}

static void bounds_adjust_kill(void* data, void* idk)
{
}

static void bounds_adjust_reset(void* data, void* idk)
{
}

static void bounds_adjust_bounce(void* data, void* idk)
{
	//if ( p->x < -)
	
}
#include <stdbool.h>

static int bounds_constrain_floor(void* data, void* pdata)
{
	VParticle*       p    = pdata;
	VParticlePlugin* plug = data;
	VPPSBoundsInfo*  inf  = plug->data;

	
	return false;
}

static int bounds_constrain_box(void* data, void* pdata)
{
	VParticle*       p    = pdata;
	VParticlePlugin* plug = data;
	VPPSBoundsInfo*  inf  = plug->data;
	double px = p->x;
	double py = p->y;
	double pz = p->z;
	
	if (p->x < -inf->bnd.x)
		p->x = -inf->bnd.x;
	if (p->x > inf->bnd.x)
		p->x = inf->bnd.x;
	if (p->y < -inf->bnd.y)
		p->y = -inf->bnd.y;
	if (p->y > inf->bnd.y)
		p->y = inf->bnd.y;
	if (p->z < -inf->bnd.z)
		p->z = -inf->bnd.z;
	if (p->z > inf->bnd.z)
		p->z = inf->bnd.z;
	
	
//	switch (inf->bounds_behavior) {
//		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL:
//
//			break;
//
//		default:
//			break;
//	}
//	if ( inf->bounds_behavior == VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_BOUNCE )
//	{
//
//	}
//
	return (px != p->x || py != p->y || pz != p->z );
	//printf("asdf\n");
}


//	todo remove this
#include <drw/drw.h>

static int bounds_constrain_sphere(void* data, void* pdata)
{
	return false;
}

#ifdef DEBUG
#include <drw/drw.h>

static void draw_debug_box(VPPSBoundsInfo* info)
{
	double x = info->bnd.x;
	double y = info->bnd.y;
	double z = info->bnd.z;
	drw_cube(info->bnd.x * 2);
	//drw_line_3f(x,y,z, -x,y,z);
	//drw_line_3f(x,-y,z, -x,-y,z);
	
}

void draw_debug(VParticlePlugin* plug)
{
	VPPSBoundsInfo* info = plug->data;
	draw_debug_box(info);
	//drw_push();
	//drw_translate(info->)
}

#endif

static void update(void* plugd, void* sysd)
{
	//printf("UPDATING\n");
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
		
		if ( violated)
		{
			
		switch (info->bounds_behavior) {
			case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL:
				free(p);
				sys->data[i] = NULL;
				break;
				
			default:
				break;
		}
		
		
		}
		//if (info->adjust_func)
		//	if ( violated )
		//		info->adjust_func(plugd, p);
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

	//free(rec);
	plug->data	= info;
	plug->destroyself = destroy;
	plug->update      = update;
	plug->draw_debug = draw_debug;
	return plug;
}
