//
//  vbl_particleplugin_spawner.h
//  vbl
//
//  Created by vs on 12/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_spawner_h
#define vbl_particleplugin_spawner_h

#include "vbl_particleplugin.h"


enum VBLParticlePluginSpawnerType
{
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_POINT,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_PLANE,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_BOX,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_SPHERE,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_NONE
};

enum VBLParticlePluginSpawnerFrequency
{
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_FRAME,
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_TIME,
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_REUSE,
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_NONE
};


typedef struct VPPSSpawnerInfo
{
	unsigned int spawn_type;
	unsigned int spawn_freq;
	double ox, oy, oz;
	double bx, by, bz;
	
	//unsigned int bounds_behavior;
} VPPSSpawnerInfo;

VPPSSpawnerInfo* vbl_particleplugin_spawnerinfo_create(void);
VParticlePlugin* vbl_particleplugin_spawner_create(VPPSSpawnerInfo* info);

#endif /* vbl_particleplugin_spawner_h */
