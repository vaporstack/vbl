//
//  vbl_particle_plugin.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin.h"

#include <stdlib.h>

VParticlePlugin* vbl_particleplugin_create(void)
{
	return calloc(1, sizeof(VParticlePlugin));
}

void vbl_particleplugin_destroy(VParticlePlugin* plug)
{
	if (plug->destroyself)
	{
		plug->destroyself(NULL, plug);
	}
	//	free(plug);
}
