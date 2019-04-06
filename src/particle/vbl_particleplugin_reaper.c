//
//  vbl_particleplugin_reaper.c
//  vbl
//
//  Created by vs on 2/8/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_reaper.h"
#include <stdlib.h>

#include <r4/src/core/r_time.h>
#include "vbl_particlesystem.h"

static void destroy(void* idk)
{
	
}

static void update(void* plugdata, void*sysdata)
{
	
	VParticleSystem* sys = sysdata;
	VParticlePlugin* plug = plugdata;
	VPPSReaperInfo* info = plug->data;
	
	double now = r_time();
	for ( unsigned i = 0, n = sys->max; i < n; i++)
	{
		VParticle* p = sys->data[i];
		if ( !p )
			continue;
		
		double d = now - p->birth;
		//printf("%.2f\n", d);
		if ( d >= info->lifespan )
		{
			//printf("chop!\n");
			free(p);
			sys->data[i] = NULL;
		}
		
	}
}

VPPSReaperInfo* vbl_particleplugin_reaperinfo_create(void)
{
	return calloc(1, sizeof(VPPSReaperInfo));

}

VParticlePlugin* vbl_particleplugin_reaper_create(VPPSReaperInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	
	plug->data = info;
	plug->update = update;
	
	return plug;
}

VParticlePlugin* vbl_particleplugin_reaper_createdefault(double lifespan)
{
	VPPSReaperInfo* info = vbl_particleplugin_reaperinfo_create();
	info->lifespan = lifespan;
	VParticlePlugin* reaper =  vbl_particleplugin_reaper_create(info);
	//vbl_particlesystem_mgr_plugin_subplugin_register(particle_mgr, reaper);
	
	return reaper;
}
