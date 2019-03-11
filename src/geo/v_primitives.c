//
//  v_primitives.c
//  vblit
//
//  Created by vs on 2/2/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "v_primitives.h"

#include <math.h>
#include <r4/r4.h>
#include <stdlib.h>

#include <vbl/vbl.h>

#pragma mark generators

RLine* v_primitives_calculate_rect(double width, double height)
{

	RLine* rect = r_line_create();
	r_line_add_point2f(rect, -width, -height);
	r_line_add_point2f(rect, width, -height);
	r_line_add_point2f(rect, width, height);
	r_line_add_point2f(rect, -width, height);
	return rect;
}

RLine* v_primitives_calculate_square(double r)
{
	return v_primitives_calculate_rect(r,r);
}

//offset is in radians I think here?
RLine* v_primitives_calculate_arc(double size, int sides, double frac, double offset)
{

	float deg2rad = (M_PI / sides);

	const int renderLineSize = (sides * 2);
	RLine*     line = r_line_create();
	line->data     = 0;
	line->num      = 0;
	line->reserved = 0;

	double limit = M_PI * 2 * frac;
	int i;
	for (i = 0; i < renderLineSize; i += 2)
	{
		float degInRad = deg2rad * (float)i;
		float x	= cos(degInRad + (M_PI * .5) + offset) * size;
		float y	= sin(degInRad + (M_PI * .5) + offset) * size;
		if ( degInRad <= limit)
			r_line_add_point2f(line, x, y);

	}
	line->closed = true;
	return line;


}

RLine* v_primitives_calculate_circle(double size, int sides, double offset)
{

	float deg2rad = (M_PI / sides);

	const int renderLineSize = (sides * 2);
	RLine*     line = r_line_create();
	line->data     = 0;
	line->num      = 0;
	line->reserved = 0;

	int i;
	for (i = 0; i < renderLineSize; i += 2)
	{
		float degInRad = deg2rad * (float)i;
		float x	= cos(degInRad + (M_PI * .5) + offset) * size;
		float y	= sin(degInRad + (M_PI * .5) + offset) * size;
		r_line_add_point2f(line, x, y);

	}
	line->closed = true;
	return line;
}

RLine* v_primitives_calculate_hex(double size, double offset)
{
	return v_primitives_calculate_circle(size, 6, offset);
}

RLine* v_primitives_calculate_triangle(double size, double offset)
{
	RLine* tri = v_primitives_calculate_circle(size, 3, offset);
	return tri;
}

RLine* v_primitives_calculate_hex_facet(double size, double offset)
{
	RLine* hex = v_primitives_calculate_hex(size, offset);

	RLine* facet = r_line_create();
	facet->num      = 0;
	facet->data     = NULL;
	facet->reserved = 0;

	r_line_add_point(facet, hex->data[0]);
	r_line_add_point(facet, hex->data[1]);
	r_line_add_point2f(facet, 0, 0);
	r_line_add_point(facet, hex->data[5]);
	r_line_destroy(hex);
	facet->closed = true;
	return facet;
}

#pragma mark placers

#include <vbl/src/core/vbl_rng.h>

RPoint3 v_primitives_random_point_on_sphere(double r)
{

	//	from:
	//	https://stackoverflow.com/questions/5531827/random-point-on-a-given-sphere/15048260#15048260
	bool   proceed = false;
	double x, y, z;
	x = y = z = 0;
	while (!proceed)
	{
		x = -.5 + r_rand_double(vbl_rng_get());
		y = -.5 + r_rand_double(vbl_rng_get());
		z = -.5 + r_rand_double(vbl_rng_get());
		if (x * x + y * y + z * z <= 1)
			proceed = true;
	}

	double d = sqrt(x * x + y * y + z * z);

	x /= d;
	y /= d;
	z /= d;

	RPoint3 p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;

}

RPoint3 v_primitives_random_point_in_sphere(double r)
{

	//	todo: sort this out, hacky lol
	RPoint3 p = v_primitives_random_point_on_sphere(r);

	double sc = r_rand_double(vbl_rng_get());
	p.x *= sc;
	p.y *= sc;
	p.z *= sc;
	return p;
}


RPoint3 v_primitives_random_point_in_box(double x, double y, double z)
{
	RPoint3 p;
	RRandom* rng = vbl_rng_get();
	p.x = r_rand_double(rng) * x;
	p.y = r_rand_double(rng) * y;
	p.z = r_rand_double(rng) * z;
	p.x -= .5 * x;
	p.y -= .5 * y;
	p.z -= .5 * z;
	return p;
}


RPoint3 v_primitives_random_point_on_box(double x, double y, double z)
{
	RPoint3 p;
	RRandom* rng = vbl_rng_get();
	int which = r_rand_double(rng) * 3;
	p.x = ( which == 0 ) ? x : r_rand_double(rng) * x;
	p.y = ( which == 1 ) ? y : r_rand_double(rng) * y;
	p.z = ( which == 2 ) ? z : r_rand_double(rng) * z;
	p.x -= .5 * x;
	p.y -= .5 * y;
	p.z -= .5 * z;
	return p;
}

void v_primitives_random_point_on_box_oldschool(double x, double y, double z, double* rx, double *ry, double* rz)
{
	//RPoint3 p;
	RRandom* rng = vbl_rng_get();
	int which = r_rand_double(rng) * 3;
	*rx = ( which == 0 ) ? x : r_rand_double(rng) * x;
	*ry = ( which == 1 ) ? y : r_rand_double(rng) * y;
	*rz = ( which == 2 ) ? z : r_rand_double(rng) * z;
	*rx -= .5 * x;
	*ry -= .5 * y;
	*rz -= .5 * z;

	//return p;
	/*
	 RPoint3 p;
	 RRandom* rng = vbl_rng_get();
	 int which = r_rand_double(rng) * 3;
	 p.x = ( which == 0 ) ? x : r_rand_double(rng) * x;
	 p.y = ( which == 1 ) ? y : r_rand_double(rng) * y;
	 p.z = ( which == 2 ) ? z : r_rand_double(rng) * z;
	 p.x -= .5 * x;
	 p.y -= .5 * y;
	 p.z -= .5 * z;
	 return p;
	 */

}
