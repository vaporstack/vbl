//
//  vbl_particlesystem_mgr.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particlesystem_mgr_h
#define vbl_particlesystem_mgr_h

#include "vbl_particlesystem.h"
#include "vbl_particlesystem_mgr_plugin.h"


typedef struct VParticleSystemMgr
{
	unsigned int	 num;
	VParticleSystemHnd** data;
	unsigned int num_plugins;
	VParticleSystemMgrPlugin** plugins;
	unsigned int num_subplugins;
	VParticlePlugin** subplugins;
	unsigned int max;
	
} VParticleSystemMgr;

#ifdef ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE
void			vbl_particlesystem_mgr_draw(VParticleSystemMgr* mgr);
#endif

VParticleSystemMgr* vbl_particlesystem_mgr_create(unsigned num, unsigned maxparticles);
void		    vbl_particlesystem_mgr_destroy(VParticleSystemMgr* mgr);
void		    vbl_particlesystem_mgr_spawn_next(VParticleSystemMgr* mgr);
void		    vbl_particlesystem_mgr_spawn(VParticleSystemMgr* mgr, unsigned int idx);
void		    vbl_particlesystem_mgr_update(VParticleSystemMgr* mgr);
void		vbl_particlesystem_mgr_plugin_register(VParticleSystemMgr* mgr, VParticleSystemMgrPlugin* plug);
unsigned 	vbl_particlesystem_mgr_count_active(VParticleSystemMgr* mgr);

//	this is for plugins that get passed to any subsequent particle system created by the manager
void		vbl_particlesystem_mgr_plugin_subplugin_register(VParticleSystemMgr* mgr, VParticlePlugin* plug);
void		vbl_particlesystem_mgr_plugin_subplugin_unregister(VParticleSystemMgr* mgr, VParticlePlugin* plug);

#endif /* vbl_particlesystem_mgr_h */
