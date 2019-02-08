//
//  vbl_particleplugin_attractor.c
//  vbl
//
//  Created by vs on 1/28/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_particleplugin_attractor.h"

#include "vbl_particlesystem.h"
#include <stdlib.h>

static void destroy(void* data, void* plugdata)
{
}

static VPPSAttractor* attractor_create(CPoint3 p, double s)
{
	VPPSAttractor* attr = calloc(1, sizeof(VPPSAttractor));

	attr->loc      = p;
	attr->strength = s;
	return attr;
}

#define TMP_SPEED .01
static void apply(VPPSAttractor* attr, VParticle* p)
{

	vec3_t a = vec3_create(NULL);
	vec3_t b = vec3_create(NULL);
	a[0]     = p->x;
	a[1]     = p->y;
	a[2]     = p->z;
	b[0]     = attr->loc.x;
	b[1]     = attr->loc.y;
	b[2]     = attr->loc.z;

	vec3_t constant = vec3_create(NULL);
	constant[0] = constant[1] = constant[2] = attr->strength;

	vec3_t dir = vec3_subtract(b, a, NULL);
	vec3_t nrm = vec3_normalize(dir, NULL);
	nrm	= vec3_multiply(nrm, constant, NULL);
	
	free(constant);
	p->ax += nrm[0] * (1. / p->mass);
	p->ay += nrm[1] * (1. / p->mass);
	p->az += nrm[2] * (1. / p->mass);

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
#include <math.h>

#include <r4/src/core/r_time.h>
static void update(void* dplug, void* dsys)
{
	
	VParticleSystem* sys  = dsys;
	VParticlePlugin* plug = dplug;

	VPPSAttractorInfo* info = plug->data;
	double t = r_time();
	
	for (unsigned i = 0; i < sys->max; i++)
	{
		VParticle* p = sys->data[i];
		if (!p)
			continue;

		for (unsigned j = 0; j < info->num; j++)
		{
			double v = .5 + sin(t*.5) * .5;
			v *= .25;
			//printf("%.2f\n", v);
			VPPSAttractor* a = info->data[j];
			a->strength = sqrt(v) * .0001;
			apply(a, p);
		}
	}
}

void vbl_particleplugin_attractor_add(VParticlePlugin* plug, CPoint3 pos, double strength)
{

	VPPSAttractorInfo* info = plug->data;
	info->num++;
	if (!info->data)
	{
		info->data = calloc(1, sizeof(VPPSAttractor));
	}
	else
	{
		info->data = realloc(info->data, info->num * sizeof(VPPSAttractor));
	}

	VPPSAttractor* att	= attractor_create(pos, strength);
	info->data[info->num - 1] = att;
	printf("Added attractor %d\n", info->num);
}

#include <drw/drw.h>

static void draw_attractor(VPPSAttractor* attr)
{
	drw_push();
	drw_translate_cp3(attr->loc);
	drw_circle(attr->strength * 100);
	drw_line_3f(0, 0, 0, 0, attr->strength, 0);
	drw_pop();
}
#ifdef DEBUG
static void draw_debug(VParticlePlugin* plug, VParticleSystem* sys)
{
	VPPSAttractorInfo* info = plug->data;
	for (int i = 0; i < info->num; i++)
	{
		VPPSAttractor* a = info->data[i];
		draw_attractor(a);
	}
}
#endif

VParticlePlugin* vbl_particleplugin_attractor_create(VPPSAttractorInfo* info)
{
	VParticlePlugin* plug = vbl_particleplugin_create();
	if (!info)
	{
		info = calloc(1, sizeof(VPPSAttractorInfo));
	}
	plug->update      = update;
	plug->data	= info;
	plug->destroyself = destroy;
#ifdef DEBUG
	plug->draw_debug = draw_debug;
#endif
	return plug;
}
