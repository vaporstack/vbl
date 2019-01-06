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
#include "vbl_particleplugin_gravity.h"
#include <stdlib.h>

VParticleSystem* vbl_particlesystem_create(void)
{
	VParticleSystem* sys = calloc(1, sizeof(VParticleSystem));

	return sys;
}

VParticleSystem* vbl_particlesystem_create_with_defaults(void)
{
	VParticleSystem* sys  = vbl_particlesystem_create();
	VParticlePlugin* grav = vbl_particleplugin_gravity_create(0, .001, 0);
	VPPSBoundsInfo   info;
	info.bounds_type     = VBL_PARTICLEPLUGIN_BOUNDSTYPE_BOX;
	info.bounds_behavior = VBL_PARTICLEPLUGIN_BOUNDSBEHAVIOR_KILL;

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
		}else{
			plug->destroyself(sys, plug);
		}
		free(plug);
	}

	for (unsigned int i = 0; i < sys->num; i++)
	{
		VParticle* p = &sys->data[i];
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
			printf("Can't update plugin!\n");
			continue;
		}

		plug->update(sys, plug);
	}
}

void vbl_particlesystem_plugin_add(VParticleSystem* sys, VParticlePlugin* plug)
{
	sys->num_plugins++;
	if (!sys->plugins)
	{
		sys->plugins = calloc(sys->num_plugins, sizeof(VParticlePlugin));
	}
	else
	{
		sys->plugins = realloc(sys->plugins, sizeof(VParticlePlugin) * sys->num_plugins);
	}
	sys->plugins[sys->num_plugins - 1] = plug;
}