//
//  vbl_particleplugin_autokill.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particlesystem_mgr_plugin_autokill.h"

#include "vbl_particlesystem_mgr.h"
#include "stdlib.h"
#include <stdbool.h>

//static void check_all_systems_for_alive_particles(
static bool check_if_any_particles_alive(VParticleSystem* sys)
{
	// bullshit obv
	//return true;
	
	for ( unsigned int i =0; i < sys->max; i++ )
	{
		VParticle* p = sys->data[i];
		if ( p )
			return true;
	}
	return false;
	
}

static void check_do_autokill(void* data, void* pdata)
{
	VParticleSystemMgr* mgr = (VParticleSystemMgr*) data;
	VParticleSystemMgrPlugin* plug = (VParticleSystemMgrPlugin*) pdata;
	VAutoKiller* killer = (VAutoKiller*)plug->data;
	switch (killer->state) {
		case VBL_PARTICLE_AUTOKILL_BEGIN:
			{
				for (int i = 0; i < mgr->num; i++ )
				{
					VParticleSystemHnd* sys = mgr->data[i];
					if ( !sys)
						continue;
					if (!sys->src)
						continue;
					if (!sys->src->data)
						continue;
					
					if ( check_if_any_particles_alive(sys->src))
						killer->state = VBL_PARTICLE_AUTOKILL_WORKING;
				}
				break;

			}
			
		default:
			break;
	}
	if ( killer->state == VBL_PARTICLE_AUTOKILL_BEGIN)
	{
		for (int i = 0; i < mgr->num; i++ )
		{
			VParticleSystemHnd* sys = mgr->data[i];
			if ( !sys)
				continue;
			
		}
	}
	

}

VParticleSystemMgrPlugin* vbl_particlesystem_mgr_plugin_autokill_create(void)
{

	VParticleSystemMgrPlugin* rec = vbl_particlesystem_mgr_plugin_create();
	rec->update = check_do_autokill;
	
	VAutoKiller* killer =calloc(1, sizeof(VAutoKiller));
	killer->state = VBL_PARTICLE_AUTOKILL_BEGIN;
	rec->data = killer;
	
	
	//rec->update	  = check_do_autokill;
	//GravityRec* grv =  calloc(1, sizeof(GravityRec));
	//grv->x = x;
	//grv->y = y;
	//grv->z = z;
	//rec->data = grv;
	return rec;
}
