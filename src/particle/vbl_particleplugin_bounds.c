//
//  vbl_particleplugin_bounds.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_bounds.h"

#include <stdlib.h>


static void bounds_check_floor(void* data, void* pdata)
{
	
}

static void bounds_check_box(void* data, void* pdata)
{
	
}
static void bounds_check_sphere(void* data, void* pdata)
{
	
}

VParticlePlugin* vbl_particleplugin_bounds_create(VPPSBoundsInfo info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	VPPSBoundsFRec* rec = calloc(1, sizeof(VPPSBoundsFRec));
	
	switch (info.bounds_type) {
		case VBL_PARTICLEPLUGIN_BOUNDSTYPE_FLOOR:
			rec->test_func = bounds_check_floor;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSTYPE_BOX:
			rec->test_func	= bounds_check_box;
			break;
		case VBL_PARTICLEPLUGIN_BOUNDSTYPE_SPHERE:
			rec->test_func = bounds_check_sphere;
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
	
	
	free(rec);
	return plug;
	
}
