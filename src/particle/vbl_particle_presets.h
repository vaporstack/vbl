//
//  vbl_particle_presets.h
//  vbl
//
//  Created by vs on 2/9/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_particle_presets_h
#define vbl_particle_presets_h

//#include "vbl_particlesystem.h"
#include "vbl_particlesystem_mgr.h"

VParticleSystem* vbl_particle_presets_rain(void);
VParticleSystem* vbl_particle_presets_snow(void);
VParticleSystem* vbl_particle_presets_fire(void);
VParticleSystem* vbl_particle_presets_dust(void);
VParticleSystem* vbl_particle_presets_explosion(void);

VParticleSystemMgr* vbl_particle_presets_mgr_explosion(void);


#endif /* vbl_particle_presets_h */
