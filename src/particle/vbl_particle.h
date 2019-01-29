//
//  vbl_particle.h
//  vbl
//
//  Created by vs on 11/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particle_h
#define vbl_particle_h

#define ALLOW_UNTIL_DRAW_DECOUPLING_COMPLETE

typedef struct VParticle
{
	double x, y, z;
	double ax, ay, az;
	double vx, vy, vz;
} VParticle;

VParticle* vbl_particle_create(void);
void       vbl_particle_destroy(VParticle* p);
void       vbl_particle_update(VParticle* p);

#endif /* vbl_particle_h */
