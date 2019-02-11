//
//  vbl_particleplugin_gravity.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_gravity_h
#define vbl_particleplugin_gravity_h

#include "vbl_particleplugin.h"

typedef struct GravityRec
{
	double x, y, z;
	
} GravityRec;

VParticlePlugin* vbl_particleplugin_gravity_create(GravityRec* rec);
VParticlePlugin* vbl_particleplugin_gravity_createdefault(double x, double y, double z);

#endif /* vbl_particleplugin_gravity_h */
