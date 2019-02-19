//
//  vbl_arc.c
//  vbl
//
//  Created by vs on 10/30/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_arc.h"

#include "../geo/v_primitives.h"
#include <drw/drw.h>

VblArc* vbl_arc_create(double width, double height, int deg, double rad, bool closed)
{
	VblArc* arc  = calloc(1, sizeof(VblArc));
	arc->width   = width;
	arc->height  = height;
	arc->degrees = deg;
	arc->closed  = closed;
	RLine* l     = v_primitives_generate_arc(width, 32, rad, 0);
	l->closed    = false;
	arc->data    = l;
	arc->r       = 0;
	arc->v       = 0;
	return arc;
}

void vbl_arc_update(VblArc* arc)
{
	arc->r += arc->v;
}

void vbl_arc_draw(VblArc* arc)
{
	if (!arc)
		return;

	drw_push();
	//drw_rotate_x(90);

	//	hack to find the 2d point?
	//drw_point_2d

	RLine* l     = arc->data;
	RPoint first = l->data[0];
	RPoint last  = l->data[l->num - 1];

	bool done = false;

	double a = arc->height * .5;
	double b = arc->height * -.5;

	drw_rotate_z(arc->r);
	drw_line_3f(first.x, first.y, a, first.x, first.y, b);
	drw_line_3f(last.x, last.y, a, last.x, last.y, b);

	drw_translate_z(a);

	drw_rline(arc->data);

	drw_translate_z(arc->height * -1);
	drw_rline(arc->data);

	drw_pop();
}

void vbl_arc_draw_fill(VblArc* arc)
{
	RLine*	l   = arc->data;
	unsigned long num = l->num * 3 * 2;
	double*       arr = calloc(num, sizeof(double));

	double a = arc->height * .5;
	double b = arc->height * -.5;
	for (int i = 0, j = 0; i < l->num; i++, j += 6)
	{
		RPoint p = l->data[i];
		double x = p.x;
		double y = p.y;
		//double z   = a;
		arr[0 + j] = x;
		arr[1 + j] = y;
		arr[2 + j] = a;
		arr[3 + j] = x;
		arr[4 + j] = y;
		arr[5 + j] = b;
	}
	drw_depth_enable();
	//drw_color_bg_set(<#double r#>, <#double g#>, <#double b#>, <#double a#>)
	//drw_color_bg();
	drw_tristrip_3d(arr, l->num);
	//drw_color_pop();
	drw_depth_disable();
	free(arr);
}
