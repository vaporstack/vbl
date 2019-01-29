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
#include "vbl_particlesystem.h"
#include <stdbool.h>

enum VBLParticlePluginSpawnerType
{
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_POINT,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_BOX,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_SPHERE,
	VBL_PARTICLEPLUGIN_SPAWNERTYPE_NONE
};

enum VBLParticlePluginSpawnerFrequency
{
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_FRAME,
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_TIME,
//	VBL_PARTICLEPLUGIN_SPAWNERFREQ_REUSE,
	VBL_PARTICLEPLUGIN_SPAWNERFREQ_NONE
};

enum VBLParticlePluginSpawnerDensity
{
	VBL_PARTICLEPLUGIN_SPAWNERDENSITY_ONE,
	VBL_PARTICLEPLUGIN_SPAWNERDENSITY_FIXED,
	VBL_PARTICLEPLUGIN_SPAWNERDENSITY_RANDOM,
	VBL_PARTICLEPLUGIN_SPAWNERDENSITY_NONE
};

typedef struct VPPSSpawnerInfo
{
	//unsigned (*spawn_num)(VParticlePlugin* plug, VParticleSystem* sys);
	void (*place)(VParticlePlugin* plug, VParticleSystem* sys, double* x, double* y, double* z);
	bool (*check)(struct VPPSSpawnerInfo* info);
	unsigned (*density)(struct VPPSSpawnerInfo* info);
	unsigned int spawn_type;
	unsigned int spawn_freq;
	unsigned int spawn_density;
	unsigned density_n;
	bool reuse;
	bool inside;
	double ox, oy, oz;
	double bx, by, bz;
	
	//unsigned int bounds_behavior;
} VPPSSpawnerInfo;

VPPSSpawnerInfo* vbl_particleplugin_spawnerinfo_create(void);
VParticlePlugin* vbl_particleplugin_spawner_create(VPPSSpawnerInfo* info);

#endif /* vbl_particleplugin_spawner_h */
