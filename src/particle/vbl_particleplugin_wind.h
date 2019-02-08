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

//typedef struct VPPSWind
//{
//	CPoint3 loc;
//	double strength;
//} VPPSAttractor;

typedef struct VPPSWindInfo
{
	int    variance;
	double vspeed;
	double vx, vy, vz;
	double vmx, vmy, vmz;
	double x, y, z;
	//VPPSAttractor** data;
	//unsigned num;
	//double strength;
} VPPSWindInfo;

VParticlePlugin* vbl_particleplugin_wind_create(VPPSWindInfo* info);
//void vbl_particleplugin_attractor_add(VParticlePlugin* plug, CPoint3 pos, double strength);

#endif /* vbl_particleplugin_wind_h */
