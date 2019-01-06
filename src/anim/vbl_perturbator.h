//
//  vbl_perturbator.h
//  vbl
//
//  Created by vs on 9/23/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_perturbator_h
#define vbl_perturbator_h

#include <stdio.h>

#include "../core/vbl_hub.h"

typedef struct VblPerturbator
{
	double frequency;
	double period;
} VblPerturbator;

void vbl_perturbator_update(VblPerturbator*pert, VblHub* hub);
void vbl_perturbator_perturb(VblPerturbator * pert, VblHub* hub);


#endif /* vbl_perturbator_h */
