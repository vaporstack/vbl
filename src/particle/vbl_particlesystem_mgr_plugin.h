//
//  vbl_particlesystem_mgr_plugin.h
//  vbl
//
//  Created by vs on 11/28/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particlesystem_mgr_plugin_h
#define vbl_particlesystem_mgr_plugin_h

typedef void (*vbl_particlesystem_mgr_plugin_fun)(void* data, void* plugdata);

typedef struct VParticleSystemMgrPlugin
{
	vbl_particlesystem_mgr_plugin_fun update;
	void* data;
}VParticleSystemMgrPlugin;

VParticleSystemMgrPlugin* vbl_particlesystem_mgr_plugin_create(void);

#endif /* vbl_particlesystem_mgr_plugin_h */
