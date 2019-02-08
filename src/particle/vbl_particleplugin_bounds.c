//
//  vbl_particleplugin_bounds.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_bounds.h"

#include <stdlib.h>
#include "vbl_particle.h"
#include "vbl_particlesystem.h"

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
	
	
}

static void bounds_constrain_floor(void* data, void* pdata)
{
	
}

static void bounds_constrain_box(void* data, void* pdata)
{
	printf("asdf\n");
}

static void bounds_constrain_sphere(void* data, void* pdata)
{
	
}

#ifdef DEBUG
#include <drw/drw.h>

static void draw_debug_box(VPPSBoundsInfo* info)
{
	
}

void draw_debug(VParticlePlugin* plug)
{
	VPPSBoundsInfo* info = plug->data;
	//drw_push();
	//drw_translate(info->)
}

#endif

static void update(void* plugd, void* sysd)
{
	printf("UPDATING\n");
	VParticleSystem* sys=sysd;
	VParticlePlugin* plug = plugd;
	VPPSBoundsInfo* info = plug->data;
	
	for ( unsigned i=0, n=sys->max; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if ( !p )
			continue;
		info->constrain_func(plugd,sysd);
		info->adjust_func(plugd, sysd);
		
		
	}
	
}

VParticlePlugin* vbl_particleplugin_bounds_create(VPPSBoundsInfo info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	//VPPSBoundsFRec* rec = calloc(1, sizeof(VPPSBoundsFRec));
	
	switch (info.bounds_type) {
		case VBL_PARTICLEPLUGIN_BOUNDSTYPE_PLANE:
			info.constrain_func = bounds_constrain_floor;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSTYPE_BOX:
			info.constrain_func	= bounds_constrain_box;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSTYPE_SPHERE:
			info.constrain_func = bounds_constrain_sphere;
			break;
		default:
			break;
	}
	
	switch (info.bounds_behavior) {
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL:
			info.adjust_func = bounds_adjust_kill;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_BOUNCE:
			info.adjust_func = bounds_adjust_bounce;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_RESET:
			info.adjust_func = bounds_adjust_reset;
			break;
			
		default:
			break;
	}
	
	
	//free(rec);
	plug->data = &info;
	plug->destroyself = destroy;
	plug->update = update;
	return plug;
	
}
