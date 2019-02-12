//
//  vbl_particleplugin_windresistance.h
//  vbl
//
//  Created by vs on 2/11/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_windresistance_h
#define vbl_particleplugin_windresistance_h

#include "vbl_particleplugin.h"

typedef struct WindResistanceInfo
{
	double drag;
} WindResistanceInfo;

VParticlePlugin* vbl_particleplugin_windresistance_create(WindResistanceInfo* info);
VParticlePlugin* vbl_particleplugin_windresistance_createdefault(double drag);

#endif /* vbl_particleplugin_windresistance_h */
