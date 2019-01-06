//
//  vbl_particle.c
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_particle.h"

#include <stdlib.h>

VParticle* vbl_particle_create(void)
{
	VParticle* p = calloc(1, sizeof(VParticle));
	return p;
}

void vbl_particle_destroy(VParticle* p)
{
	free(p);
}
