//
//  vbl_particleplugin_wind.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_wind_h
#define vbl_particleplugin_wind_h

#include "vbl_particleplugin.h"


typedef struct VPPSWindInfo
{
	int    variance;
	double vspeed;
	double vx, vy, vz;
	double vmx, vmy, vmz;
	double x, y, z;

} VPPSWindInfo;

VParticlePlugin* vbl_particleplugin_wind_create(VPPSWindInfo* info);
VParticlePlugin* vbl_particleplugin_wind_createdefault(int varying, double vx, double vy, double vz);

#endif /* vbl_particleplugin_wind_h */
