//
//  vbl_particleplugin_attractor.h
//  vbl
//
//  Created by vs on 1/28/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_attractor_h
#define vbl_particleplugin_attractor_h

#include <coer/coer.h>
#include "vbl_particleplugin.h"

typedef struct VPPSAttractor
{
	CPoint3 loc;
	double strength;
} VPPSAttractor;

typedef struct VPPSAttractorInfo
{
	VPPSAttractor** data;
	unsigned num;
	//double strength;
}VPPSAttractorInfo;

VParticlePlugin* vbl_particleplugin_attractor_create(VPPSAttractorInfo* info);
void vbl_particleplugin_attractor_add(VParticlePlugin* plug, CPoint3 pos, double strength);

#endif /* vbl_particleplugin_attractor_h */
