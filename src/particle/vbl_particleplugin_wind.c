//
//  vbl_particleplugin_wind.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_wind.h"
#include "vbl_particlesystem.h"

//	todo: transition to a plugin updating an individual particle so we
//	can eventually do them all in parallel?

static void update_one(VParticlePlugin* plug, VParticle* p)
{
	VPPSWindInfo* info = plug->data;
	
	p->ax += info->x;
	p->ay += info->y;
	p->az += info->z;
}

static void update(void* pdata, void* sdata)
{
	VParticlePlugin* plug = pdata;
	VParticleSystem* sys = sdata;
	VPPSWindInfo* info = plug->data;

	for ( unsigned i = 0; i < sys->max; i++ )
	{
		VParticle* p = sys->data[i];
		if ( !p )
			continue;
		p->ax += info->x;
		p->ay += info->y;
		p->az == info->z;
		
	}
	
}

VParticlePlugin* vbl_particleplugin_wind_create(VPPSWindInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	
	plug->data = info;
	plug->update = update;
	
	return plug;
	
}
