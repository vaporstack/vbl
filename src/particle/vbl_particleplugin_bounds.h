//
//  vbl_particleplugin_bounds.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_bounds_h
#define vbl_particleplugin_bounds_h

#include "vbl_particleplugin.h"

enum VBLParticlePluginBoundsType
{
	VBL_PARTICLEPLUGIN_BOUNDSTYPE_FLOOR,
	VBL_PARTICLEPLUGIN_BOUNDSTYPE_BOX,
	VBL_PARTICLEPLUGIN_BOUNDSTYPE_SPHERE,
	VBL_PARTICLEPLUGIN_BOUNDSTYPE_NONE
};

enum VBLParticlePluginBoundsBehavior
{
	VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL,
	VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_BOUNCE,
	VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_RESET,
	VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_NONE

};

typedef struct VPPSBoundsInfo
{
	unsigned int bounds_type;
	unsigned int bounds_behavior;
} VPPSBoundsInfo;

typedef struct VPPSBoundsFRec
{
	vbl_particle_plugin_fun kill_func;
	vbl_particle_plugin_fun test_func;
}VPPSBoundsFRec;

VParticlePlugin* vbl_particleplugin_bounds_create(VPPSBoundsInfo info);

#endif /* vbl_particleplugin_bounds_h */
