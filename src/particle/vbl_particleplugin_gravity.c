//
//  vbl_particleplugin_gravity.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_gravity.h"

#include "vbl_particlesystem.h"
#include <stdlib.h>

typedef struct GravityRec
{
	double x, y, z;

} GravityRec;

static void apply_gravity(void* data, void* pdata)
{
	GravityRec*      grv = (GravityRec*)pdata;
	VParticleSystem* sys = (VParticleSystem*)data;
	for (unsigned int i = 0; i < sys->num; i++)
	{
		VParticle* p = &sys->data[i];
		if (!p)
			continue;
		p->x += grv->x;
		p->y += grv->y;
		p->z += grv->z;
	}
}

VParticlePlugin* vbl_particleplugin_gravity_create(double x, double y, double z)
{

	VParticlePlugin* rec = vbl_particleplugin_create();
	rec->update	  = apply_gravity;
	GravityRec* grv      = calloc(1, sizeof(GravityRec));
	grv->x		     = x;
	grv->y		     = y;
	grv->z		     = z;
	rec->data	    = grv;
	return rec;
}
