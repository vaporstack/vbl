//
//  vbl_particleplugin_spawner.c
//  vbl
//
//  Created by vs on 12/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_spawner.h"

#include <stdlib.h>

VPPSSpawnerInfo* vbl_particleplugin_spawnerinfo_create(void)
{
	VPPSSpawnerInfo* info = calloc(1, sizeof(VPPSSpawnerInfo));
	
	return info;
}

VParticlePlugin* vbl_particleplugin_spawner_create(VPPSSpawnerInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	plug->data = info;
	
	return plug;
}
