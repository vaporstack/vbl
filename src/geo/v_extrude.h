
//  v_extrude.c
//  v_extrude
//
//  Created by Andrew Macfarlane on 30/07/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_extrude_h_
#define v_extrude_h_

// class to take some 2d geo and build a mesh? out of it
//	or an object at any rate

#include <r4/r4.h>

typedef struct VExtrude
{
	vec3_t  position;
	double *x_coords;
	double *y_coords;
	double *z_coords;

} VExtrude;

VExtrude *v_extrude_create(RLine *line);
void      v_extrude_destroy(VExtrude *extrusion);

#endif
