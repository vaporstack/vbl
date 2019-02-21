//
//  vbl_branch.h
//  vbl
//
//  Created by vs on 2/21/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_branch_h
#define vbl_branch_h

#include "../particle/vbl_particlesystem.h"
#include <r4/src/geo/r_line.h>
#include <coer/coer.h>

typedef struct VBranch
{
	RLine* line;
	//vec3_t	orientation;

} VBranch;

typedef struct VBranchSys
{
	VBranch**	data;
	unsigned int     num;
	VParticleSystem* sys;

} VBranchSys;

VBranch*    vbl_branch_create(void);
void vbl_branch_destroy(VBranch* branch);
void vbl_branch_update(VBranch* branch, VParticle* p);
VBranchSys* vbl_branch_sys_create(unsigned int num);
void	vbl_branch_sys_update(VBranchSys* sys);
void	vbl_branch_sys_destroy(VBranchSys* sys);

#endif /* vbl_branch_h */
