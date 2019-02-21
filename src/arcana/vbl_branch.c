//
//  vbl_branch.c
//  vbl
//
//  Created by vs on 2/21/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_branch.h"

#include <vbl/src/core/vbl_rng.h>
#include <r4/src/core/r_random.h>

VBranch* vbl_branch_create(void)
{
	VBranch* b = calloc(1, sizeof(VBranch));
	return b;
}
#define TMP_SMALL_CONST .000001

void vbl_branch_update(VBranch* branch, VParticle* p)
{
	//if ( !branch->orientation )
	//	branch->orientation = vec3_create(NULL);

	if ( !branch->line)
	{
		branch->line = r_line3_create();
	}
	r_line3_add_point3f(branch->line, p->x, p->y, p->z);

	RRandom* rng = vbl_rng_get();
	double ex = -.5 + r_rand_double(rng);
	double ey = -.5 + r_rand_double(rng);
	double ez = -.5 + r_rand_double(rng);
	double sc = .25 / 3;
	ex *= sc;
	ey *= sc;
	ez *= sc;
	
	p->vx += ex * TMP_SMALL_CONST;
	p->vy += ey * TMP_SMALL_CONST;
	p->vz += ez * TMP_SMALL_CONST;
	
//	branch->orientation[0] += ex;
//	branch->orientation[1] += ey;
//	branch->orientation[2] += ez;
//
	
	
	//	
	
}

void vbl_branch_destroy(VBranch* branch)
{
	//if ( branch->orientation)
	//	free(branch->orientation);
	if ( branch->line)
		r_line_destroy(branch->line);
	free(branch);
}

VBranchSys* vbl_branch_sys_create(unsigned int num)
{
	VBranchSys* bsys = calloc(1, sizeof(VBranchSys));
	bsys->sys	= vbl_particlesystem_create(num);
	bsys->data = calloc(num, sizeof(VBranch));
	bsys->num = num;
	for ( unsigned i = 0, n=num; i < n; i++ )
	{
		bsys->data[i] = vbl_branch_create();
	}
	return bsys;
}


void vbl_branch_sys_update(VBranchSys* bsys)
{
	vbl_particlesystem_update(bsys->sys);
	for (unsigned i = 0, n = bsys->num; i < n; i++)
	{
		VBranch*   b = bsys->data[i];

		VParticle* p = bsys->sys->data[i];
		if ( !p )
			continue;
		
		vbl_branch_update(b, p);
		//p->ax += TMP_SMALL_CONST * b->orientation[0];
		//p->ay += TMP_SMALL_CONST * b->orientation[1];
		//p->az += TMP_SMALL_CONST * b->orientation[2];
		vbl_particle_update(p);
		

	}
}

void vbl_branch_sys_destroy(VBranchSys* bsys)
{
	for ( unsigned i = 0, n = bsys->num; i<n; i++ )
	{
		VBranch* b = bsys->data[i];
		vbl_branch_destroy(b);
		free(b);
	}
	
	vbl_particlesystem_destroy(bsys->sys);
}
