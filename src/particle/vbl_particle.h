//
//  vbl_particle.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particle_h
#define vbl_particle_h

typedef struct VParticle
{
	double x, y, z;
} VParticle;

VParticle* vbl_particle_create(void);
void       vbl_particle_destroy(VParticle* p);

#endif /* vbl_particle_h */
