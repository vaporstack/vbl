//
//  vbl_particlesystem.c
//  vbl
//
//  Created by vs on 11/24/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "vbl_particlesystem.h"
#include "vbl_particleplugin.h"

#include "vbl_particleplugin_bounds.h"
#include "vbl_particleplugin_emitter.h"
#include "vbl_particleplugin_gravity.h"
#include <stdlib.h>

VParticleSystem* vbl_particlesystem_create(unsigned max)
{
	
	VParticleSystem* sys = calloc(1, sizeof(VParticleSystem));
	sys->data	    = calloc(max, sizeof(VParticle*));
	sys->max	     = max;
	return sys;
}

VParticleSystem* vbl_particlesystem_create_with_defaults(unsigned max)
{
	VParticleSystem* sys  = vbl_particlesystem_create(max);
	VParticlePlugin* grav = vbl_particleplugin_gravity_createdefault(0, .001, 0);
	VPPSBoundsInfo *  info = calloc(1,sizeof(VPPSBoundsInfo));
	info->bounds_type     = VBL_PARTICLEPLUGIN_BOUNDSTYPE_BOX;
	info->bounds_behavior = VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL;

	VParticlePlugin* bounds = vbl_particleplugin_bounds_create(info);

	vbl_particlesystem_plugin_add(sys, grav);
	vbl_particlesystem_plugin_add(sys, bounds);
	return sys;
}

void vbl_particlesystem_destroy(VParticleSystem* sys)
{
	for (unsigned int i = 0; i < sys->num_plugins; i++)
	{
		VParticlePlugin* plug = sys->plugins[i];
		if (!plug)
		{
			printf("AAAAAERROR\n");
			continue;
		}
		if (!plug->destroyself)
		{
			printf("Can't invoke destroy function, was not provided!\n");
		}
		else
		{
			plug->destroyself(sys, plug);
		}
		free(plug);
	}

	for (unsigned int i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;
	}
	if (sys)
		free(sys);
}
/*
void vbl_particlesystem_draw(VParticleSystem* sys)
{
	for ( unsigned i = 0; i < sys->num; i++ )
	{
		VParticle* p = &sys->data[i];
		if ( !p )
			continue;
		if ( sys->draw )
			sys->draw(sys);

	}
}*/

void vbl_particlesystem_update(VParticleSystem* sys)
{
	for (int i = 0; i < sys->num_plugins; i++)
	{
		VParticlePlugin* plug = sys->plugins[i];
		if (!plug->update)
		{
			//toDO fix this:
			printf("Can't update plugin!\n");
			continue;
		}

		plug->update(plug, sys);
	}
	for (unsigned i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;
		vbl_particle_update(p);
	}
}

#include "../core/vbl_log.h"

void vbl_particlesystem_plugin_add(VParticleSystem* sys, VParticlePlugin* plug)
{
	sys->num_plugins++;
	if (!sys->plugins)
	{
		sys->plugins = (void*)calloc(sys->num_plugins, sizeof(VParticlePlugin*));
	}
	else
	{
		sys->plugins = realloc(sys->plugins, sizeof(VParticlePlugin) * sys->num_plugins);
	}
	sys->plugins[sys->num_plugins - 1] = plug;
	
	//	sneak a copy of the emitter for ourselves! we'll need it later. lol
	//VPPSEmitInfo* info = (VPPSEmitInfo*)plug->data;
	if ( plug->type == VBL_PARTICLEPLUGIN_TYPE_EMITTER )
	{
		//	it worked! yoink
		vbl_log("yoink!");
		sys->emitter = plug;
	}

}

bool vbl_particlesystem_any_alive(VParticleSystem* sys)
{
	for (unsigned i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (p)
			return true;
	}
	return false;
}

signed vbl_particlesystem_next_available(VParticleSystem* sys)
{
	for (unsigned i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
		{
			sys->pos = i;
			return sys->pos;
		}
	}
	return -1;
}

signed vbl_particlesystem_next(VParticleSystem* sys)
{
	sys->pos++;
	if ( sys->pos > sys->max)
	{
		printf("I never expected to be here.\n");
		
	}
	if (sys->pos == sys->max)
		sys->pos = 0;
	
	//VParticle* p = sys->data[sys->pos];
	if ( sys->data[sys->pos] )
	{
		free(sys->data[sys->pos]);
		sys->data[sys->pos] = NULL;
	}
	return sys->pos;
}
