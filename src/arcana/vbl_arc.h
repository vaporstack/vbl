//
//  vbl_arc.h
//  vbl
//
//  Created by vs on 10/30/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_arc_h
#define vbl_arc_h

#include <stdbool.h>
#include <stdio.h>

typedef struct VblArc
{
	int    degrees;
	double width;
	double height;
	double ang;
	double r;
	double closed;
	double v;
	void*  data;
} VblArc;

VblArc* vbl_arc_create(double width, double height, int deg, double rad, bool closed);
void    vbl_arc_update(VblArc* arc);
void    vbl_arc_draw(VblArc* arc);
void    vbl_arc_draw_fill(VblArc* arc);

#endif /* vbl_arc_h */
