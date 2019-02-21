//
//  vbl_particle_plugin.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particle_plugin_h
#define vbl_particle_plugin_h

typedef void (*vbl_particle_plugin_fun)(void* plugdata, void* sysdata);
typedef int (*vbl_particle_plugin_constrain_func)(void* plugdata, void* sysdata);
typedef void (*vbl_particle_plugin_test_fun)(void* plugdata, void* sysdata, void* pdata);
typedef void (*vbl_particle_plugin_single_fun)(void* plugdata, void* sysdata, void* pdata);


//	placeholder, I could get clever with a factory later like I did for gentities maybe
enum
{
	VBL_PARTICLEPLUGIN_TYPE_NONE,
	VBL_PARTICLEPLUGIN_TYPE_EMITTER,
	VBL_PARTICLEPLUGIN_TYPE_BOUNDS
};

typedef struct VParticlePlugin
{
	unsigned type;
	vbl_particle_plugin_fun update;
	vbl_particle_plugin_fun destroyself;
	void*			data;
#ifdef DEBUG
	vbl_particle_plugin_fun draw_debug;
#endif

} VParticlePlugin;

VParticlePlugin* vbl_particleplugin_create(void);
void		 vbl_particleplugin_destroy(VParticlePlugin* plug);

#endif /* vbl_particle_plugin_h */
