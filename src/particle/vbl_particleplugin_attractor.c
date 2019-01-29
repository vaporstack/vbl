//
//  vbl_particleplugin_attractor.c
//  vbl
//
//  Created by vs on 1/28/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_attractor.h"

#include <stdlib.h>
#include "vbl_particlesystem.h"

static void destroy(void* data, void* plugdata)
{
	
}

static VPPSAttractor* attractor_create(CPoint3 p, double s)
{
	VPPSAttractor* attr = calloc(1, sizeof(VPPSAttractor));
	
	attr->loc = p;
	attr->strength = s;
	return attr;
}

#define TMP_SPEED .01
static void apply(VPPSAttractor* attr, VParticle* p)
{
	
	
	vec3_t a=  vec3_create(NULL);
	vec3_t b=vec3_create(NULL);
	a[0] = p->x;
	a[1] = p->y;
	a[2] = p->z;
	b[0] = attr->loc.x;
	b[1] = attr->loc.y;
	b[2] = attr->loc.z;
	
	vec3_t constant = vec3_create(NULL);
	constant[0] = constant[1] = constant[2] = attr->strength;
	
	vec3_t dir = vec3_subtract(b, a, NULL);
	vec3_t nrm = vec3_normalize(dir, NULL);
	nrm = vec3_multiply(nrm, constant, NULL);
	
	p->ax += nrm[0];
	p->ay += nrm[1];
	p->az += nrm[2];
	
	
	free(a);
	free(b);
//	double dx = p->x - attr->loc.x;
//	double dy = p->y - attr->loc.y;
//	double dz = p->z - attr->loc.z;
//
//	p->x += -dx * TMP_SPEED;
//	p->y += -dy * TMP_SPEED;
//	p->z += -dz * TMP_SPEED;
//
	
}

static void update(void* dsys, void* dplug)
{
	VParticleSystem* sys = dsys;
	VParticlePlugin* plug = dplug;

	VPPSAttractorInfo* info = plug->data;

	for ( unsigned i = 0; i < sys->max; i++ )
	{
		VParticle* p = sys->data[i];
		if ( !p )
			continue;
		
		for ( unsigned j = 0; j < info->num;j++ )
		{
			VPPSAttractor * a = info->data[j];
			apply(a, p);
		}
	
	}
}

void vbl_particleplugin_attractor_add(VParticlePlugin* plug, CPoint3 pos, double strength)
{
	
	VPPSAttractorInfo* info = plug->data;
	info->num++;
	if ( !info->data )
	{
		info->data = calloc(1, sizeof(VPPSAttractor));
	}else{
		info->data = realloc(info->data, info->num * sizeof(VPPSAttractor));
	}
	
	VPPSAttractor* att = attractor_create(pos , strength);
	info->data[info->num -1 ] = att;
	printf("Added attractor %d\n", info->num);
}

VParticlePlugin* vbl_particleplugin_attractor_create(VPPSAttractorInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	if ( !info )
	{
		info = calloc(1, sizeof(VPPSAttractorInfo));
		
	}
	plug->update = update;
	plug->data = info;
	plug->destroyself = destroy;
	return plug;
}
