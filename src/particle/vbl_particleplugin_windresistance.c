//
//  vbl_particleplugin_windresistance.c
//  vbl
//
//  Created by vs on 2/11/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_windresistance.h"

#include <stdlib.h>
#include "vbl_particlesystem.h"


static void update_windresistance(void* plugd, void* sysd)
{
	VParticlePlugin* plug = plugd;
	VParticleSystem* sys = sysd;
	WindResistanceInfo* info = plug->data;
	for ( unsigned i =0, n=sys->max; i <n ; i++)
	{
		VParticle* p = sys->data[i];
		if ( !p )
			continue;
		
		p->vx *= info->drag;
		p->vy *= info->drag;
		p->vz *= info->drag;
	}
}

VParticlePlugin* vbl_particleplugin_windresistance_create(WindResistanceInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	plug->update	  = update_windresistance;
	plug->data	    = info;
	return plug;
}

VParticlePlugin* vbl_particleplugin_windresistance_createdefault(double drag)
{
	WindResistanceInfo* info = calloc(1, sizeof(WindResistanceInfo));
	info->drag		 = drag;
	VParticlePlugin* plug    = vbl_particleplugin_windresistance_create(info);

	return plug;
}
