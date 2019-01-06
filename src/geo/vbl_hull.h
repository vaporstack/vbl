
//  vbl_hull.c
//  vbl_hull
//
//  Created by Andrew Macfarlane on 10/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef vbl_hull_h_
#define vbl_hull_h_

#include <r4/src/geo/r_line.h>

typedef struct RHull
{
	RLine* base;
	RLine* convex;
} RHull;

RHull* vbl_hull_create(RLine* base);
void   vbl_hull_destroy(RHull* hull);

void vbl_hull_approximate(RHull* hull);

#endif
