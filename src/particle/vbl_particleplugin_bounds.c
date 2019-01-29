//
//  vbl_particleplugin_bounds.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_bounds.h"

#include <stdlib.h>

static void destroy(void* idk1, void* idk2)
{
	
}

static void bounds_constrain_floor(void* data, void* pdata)
{
	
}

static void bounds_constrain_box(void* data, void* pdata)
{
	
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
			//	do stuff
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_BOUNCE:
			//	do stuff
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_RESET:
			//	do stuff
			break;
			
		default:
			break;
	}
	
	
	//free(rec);
	plug->destroyself = destroy;
	return plug;
	
}
