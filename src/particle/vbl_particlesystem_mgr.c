//
//  vbl_particlesystem_mgr.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particlesystem_mgr.h"

#include <stdlib.h>

VParticleSystemMgr* vbl_particlesystem_mgr_create(unsigned int num, unsigned int max)
{
	VParticleSystemMgr* mgr = calloc(1, sizeof(VParticleSystemMgr));
	mgr->num		= num;
	mgr->data		= calloc(num, sizeof(VParticleSystemHnd*));
	for (unsigned int i = 0; i < mgr->num; i++)
	{
		mgr->data[i]      = calloc(1, sizeof(VParticleSystemHnd));
		mgr->data[i]->src = NULL; //redundant because we're using calloc?
	}
	mgr->max = max;
	return mgr;
}

unsigned vbl_particlesystem_mgr_count_active(VParticleSystemMgr* mgr)
{
	unsigned res = 0;
	for (unsigned int i = 0; i < mgr->num; i++)
	{
		if (mgr->data[i]->src)
			res++;
	}
	return res;
}

void vbl_particlesystem_mgr_destroy(VParticleSystemMgr* mgr)
{
	for (unsigned int i = 0; i < mgr->num_subplugins; i++)
	{
		VParticlePlugin* plug = mgr->subplugins[i];
		vbl_particleplugin_destroy(plug);
	}

	for (unsigned int i = 0; i < mgr->num; i++)
	{
		VParticleSystemHnd* hnd = mgr->data[i];
		VParticleSystem*    src = hnd->src;
		if (src)
		{
			vbl_particlesystem_destroy(src);
		}
		hnd->src = NULL;
		free(hnd);
	}
	free(mgr);
}

void vbl_particlesystem_mgr_update(VParticleSystemMgr* mgr)
{
	//	todo: get this out of here this spawns a new system on every frame wtf!!
	//vbl_particlesystem_mgr_spawn_next(mgr);

	//	so here we wana run through the uber plugins

	for (unsigned int i = 0; i < mgr->num_plugins; i++)
	{
		VParticleSystemMgrPlugin* plug = mgr->plugins[i];
		plug->update(mgr, plug);
	}

	//	then, update the active systems (if we have them)
	for (unsigned int i = 0; i < mgr->num; i++)
	{
		VParticleSystemHnd* hnd = mgr->data[i];
		VParticleSystem*    sys = hnd->src;
		if (!sys)
			continue;

		vbl_particlesystem_update(sys);
	}
}

//	todo:
//	currently this assumes ownership. should probably rename to add/remove instead of register/unregister
void vbl_particlesystem_mgr_plugin_subplugin_register(VParticleSystemMgr* mgr, VParticlePlugin* plug)
{
	if (mgr->num_subplugins == 0)
	{
		mgr->subplugins = calloc(1, sizeof(VParticlePlugin*));
	}
	else
	{
		mgr->subplugins = realloc(mgr->subplugins, sizeof(VParticleSystemMgrPlugin) * mgr->num_subplugins + 1);
	}
	mgr->num_subplugins++;
	mgr->subplugins[mgr->num_subplugins - 1] = plug;
}

void vbl_particlesystem_mgr_plugin_subplugin_unregister(VParticleSystemMgr* mgr, VParticlePlugin* plug)
{
	//	do something? we're killing all these anyway
}

static int get_next_available_system_slot(VParticleSystemMgr* mgr)
{
	for (unsigned int i = 0; i < mgr->num; i++)
	{
		VParticleSystemHnd* hnd = mgr->data[i];
		if (hnd->src == NULL)
			return i;
	}
	return -1;
}

void vbl_particlesystem_mgr_spawn(VParticleSystemMgr* mgr, unsigned int idx)
{
	VParticleSystemHnd* hnd = mgr->data[idx];
	if (hnd->src != NULL)
	{
		printf("Error, tried to spawn on top of an existing system.\n");
		return;
	}
	VParticleSystem* sys = vbl_particlesystem_create(mgr->max);
	for (unsigned int i = 0; i < mgr->num_subplugins; i++)
	{
		vbl_particlesystem_plugin_add(sys, mgr->subplugins[i]);
	}
	hnd      = calloc(1, sizeof(VParticleSystemHnd));
	hnd->src = sys;

	mgr->data[idx] = hnd;
}
void vbl_particlesystem_mgr_spawn_next(VParticleSystemMgr* mgr)
{
	int which = get_next_available_system_slot(mgr);
	if (which != -1)
	{
		printf("Spawning new system at slot %d\n", which);

		vbl_particlesystem_mgr_spawn(mgr, which);
	}
}

void vbl_particlesystem_mgr_plugin_register(VParticleSystemMgr* mgr, VParticleSystemMgrPlugin* plug)
{
	if (mgr->num_plugins == 0)
	{
		mgr->plugins = calloc(1, sizeof(VParticleSystemMgrPlugin*));
	}
	else
	{
		mgr->plugins = realloc(mgr->plugins, sizeof(VParticlePlugin) * mgr->num_plugins + 1);
	}
	mgr->num_plugins++;
	mgr->plugins[mgr->num_plugins - 1] = plug;
}

#ifdef ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE
#include <drw/drw.h>
static void error_draw(VParticleSystem* sys)
{
	for (unsigned i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;
		drw_push();
		drw_translate(p->x, p->y, p->z);
		drw_scale_u(.1);
		drw_square(.001 * p->mass);
		//drw_rect(-.5,-.5, .5,1);
		drw_point();
		drw_pop();
	}

#ifdef DEBUG
	for (unsigned i = 0; i < sys->num_plugins; i++)
	{
		VParticlePlugin* plug = sys->plugins[i];
		if (plug->draw_debug)
			plug->draw_debug(plug, sys);
	}
#endif
}

void vbl_particlesystem_mgr_draw(VParticleSystemMgr* mgr)
{

	for (unsigned int i = 0; i < mgr->num; i++)
	{
		VParticleSystem* sys = mgr->data[i]->src;
		if (!sys)
		{
			//printf("can't draw sys no sys\n");
			continue;
		}

		if (!sys->draw)
		{
			printf("Error, had a system with no draw function.\n");
			error_draw(sys);
			continue;
		}

		sys->draw(sys);
	}
}
#endif
