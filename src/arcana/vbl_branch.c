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
#include <r4/src/core/r_math.h>

VBranch* vbl_branch_create(void)
{
	VBranch* b = calloc(1, sizeof(VBranch));
	return b;
}
#define TMP_SMALL_CONST .0001

void vbl_branch_update(VBranch* branch, VParticle* p)
{
	//if ( !branch->orientation )
	//	branch->orientation = vec3_create(NULL);
//
//	if ( branch->line)
//	{
//		double d = fabs(dist3d(p->px, p->py, p->pz, p->x, p->y, p->z ));
//		if ( d > .05)
//		{
//			//hack to break these things based on travel distance, seems simpler than adding
//			//	callback infrastructure to particles mehhhhh
//			r_line3_destroy(branch->line);
//			printf("snap!\n");
//		}
//	}
	
	if ( !branch->line)
	{
		////branch->line = r_line3_create();
		branch->line = wsh_line_create();
	}
	double sum = wsh_line_sum(branch->line);
	//printf("%.2f\n", sum);
	if ( sum >= 1 )
		return;
	
	//r_line3_add_point3f(branch->line, p->x, p->y, p->z);
	wsh_line_add_point2f(branch->line, p->x, p->y);
	
	RRandom* rng = vbl_rng_get();
	double ex = -.5 + r_rand_double(rng);
	double ey = -.5 + r_rand_double(rng);
	double ez = -.5 + r_rand_double(rng);
	double sc = .25 / 3;
	ex *= sc;
	ey *= sc;
	ez *= sc;
	ez = 0;
	p->vx += ex * TMP_SMALL_CONST;
	p->vy += ey * TMP_SMALL_CONST;
	p->vz += ez * TMP_SMALL_CONST;

	
	for ( unsigned i = 0 ;i < branch->line->num; i++ )
	{
		if ( r_rand_double(rng) < .333)
			branch->line->data[i].pressure += .00001;
		if ( r_rand_double(rng) < .333)
			branch->line->data[i].pressure += (-.5 + r_rand_double(rng)) * .001;
	}

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
		wsh_line_destroy(branch->line);
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
