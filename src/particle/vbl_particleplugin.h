//
//  vbl_particle_plugin.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particle_plugin_h
#define vbl_particle_plugin_h

typedef void (*vbl_particle_plugin_fun)(void* data, void* plugdata);
typedef void (*vbl_particle_plugin_test_fun)(void* data, void* plugdata, void* pdata);

typedef struct VParticlePlugin
{
	vbl_particle_plugin_fun update;
	vbl_particle_plugin_fun destroyself;
	void*			data;
} VParticlePlugin;

VParticlePlugin* vbl_particleplugin_create(void);
void		 vbl_particleplugin_destroy(VParticlePlugin* plug);

#endif /* vbl_particle_plugin_h */