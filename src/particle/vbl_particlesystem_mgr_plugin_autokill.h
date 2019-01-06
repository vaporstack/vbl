//
//  vbl_particleplugin_autokill.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_autokill_h
#define vbl_particleplugin_autokill_h

#include "vbl_particlesystem_mgr_plugin.h"


enum VBLParticleAutoKillStatus
{
	VBL_PARTICLE_AUTOKILL_BEGIN,
	VBL_PARTICLE_AUTOKILL_WORKING,
	VBL_PARTICLE_AUTOKILL_DONE
};

typedef struct VAutoKiller
{
	int state;
}VAutoKiller;

VParticleSystemMgrPlugin * vbl_particlesystem_mgr_plugin_autokill_create(void);

#endif /* vbl_particleplugin_autokill_h */
