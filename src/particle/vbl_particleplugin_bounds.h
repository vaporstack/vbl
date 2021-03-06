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
#include <coer/coer.h>

enum VBLParticlePluginBoundsType
{
	VBL_PARTICLEPLUGIN_BOUNDSTYPE_PLANE,
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
	unsigned int			   bounds_type;
	unsigned int			   bounds_behavior;
	unsigned int			   bounds_volume;
	vbl_particle_plugin_single_fun	   adjust_func;
	vbl_particle_plugin_constrain_func constrain_func;
	double bounce_drag;
	CPoint3				   pos;
	CPoint3				   bnd;
	double radius;
} VPPSBoundsInfo;

//
//typedef struct VPPSBoundsFRec
//{
//	vbl_particle_plugin_fun modify_func;
//	vbl_particle_plugin_fun constrain_func;
//}VPPSBoundsFRec;

VPPSBoundsInfo* vbl_particleplugin_boundsinfo_create(void);
VParticlePlugin* vbl_particleplugin_bounds_create(VPPSBoundsInfo* info);
VParticlePlugin* vbl_particleplugin_bounds_createdefault(VPPSBoundsInfo* info, double x, double y, double z);

#endif /* vbl_particleplugin_bounds_h */
