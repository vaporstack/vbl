//
//  vbl_particleplugin_reaper.h
//  vbl
//
//  Created by vs on 2/8/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_reaper_h
#define vbl_particleplugin_reaper_h

#include "vbl_particleplugin.h"

typedef struct VPPSReaperInfo
{
	double lifespan;
} VPPSReaperInfo;

VPPSReaperInfo* vbl_particleplugin_reaperinfo_create(void);
VParticlePlugin* vbl_particleplugin_reaper_create(VPPSReaperInfo* info);
VParticlePlugin* vbl_particleplugin_reaper_createdefault(double lifespan);

#endif /* vbl_particleplugin_reaper_h */
