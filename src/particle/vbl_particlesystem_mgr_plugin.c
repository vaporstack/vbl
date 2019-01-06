//
//  vbl_particlesystem_mgr_plugin.c
//  vbl
//
//  Created by vs on 11/28/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particlesystem_mgr_plugin.h"

#include <stdlib.h>


VParticleSystemMgrPlugin* vbl_particlesystem_mgr_plugin_create(void)
{
	VParticleSystemMgrPlugin* rec = calloc(1, sizeof(VParticleSystemMgrPlugin));
	
	return rec;
	
}
