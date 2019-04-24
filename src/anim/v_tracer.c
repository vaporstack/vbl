//
//  v_tracer.c
//  vbl
//
//  Created by vs on 2/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "v_tracer.h"
#include <drw/drw.h>
#include <r4/r4.h>

#include <wsh/src/util/wsh_math.h>

#include <stdio.h>

static void set_heading(WTracerLinear* tracer);
static void collide(WTracerLinear* tracer);

WTracerLinear* v_WTracerLinear_create(WLine* line)
{
	if (line->num < 2)
	{
		printf("Can't create a tracer without at least 2 points!\n");
		return NULL;
	}
	WTracerLinear* t = calloc(1, sizeof(WTracerLinear));
	printf("copying the first point to the end\n");

	wsh_line_add_point(line, line->data[0]);
	t->hnd.src = line;
	t->active  = false;
	t->speed   = 1.;
	t->index   = 0;
	t->loop    = true;
	return t;
}

static void collide(WTracerLinear* tracer)
{
	//printf("we reached the next vert!\n");
	tracer->index++;

	set_heading(tracer);
}

static void set_heading(WTracerLinear* tracer)
{
	WLine* l = tracer->hnd.src;

	if (tracer->index + 1 >= l->num)
	{
		//printf("hit loop!");
		tracer->index = 0;
	}

	WPoint a    = l->data[tracer->index];
	int    next = tracer->index + 1;
	WPoint b    = l->data[next];

	if (a.x == b.x && a.y == b.y)
	{
		//	skip a point!
		//printf("ooops this shouldn't happen\n");
		tracer->index += 1;
		set_heading(tracer);
		return;
	}
	//tracer->prev = a;
	tracer->cursor = a;
	tracer->next   = b;
	double ang     = wsh_angle_from_points_wp_r(a, b);

	//printf("Moving from %d to %d\n", tracer->index, next);
	//printf("ang is %f\n", ang);
	tracer->heading.x = cos(ang) * tracer->speed;
	tracer->heading.y = sin(ang) * tracer->speed;
}

void v_WTracerLinear_start(WTracerLinear* tracer)
{
	set_heading(tracer);
}

void v_WTracerLinear_destroy(WTracerLinear* tracer)
{
	free(tracer);
}

void v_WTracerLinear_update(WTracerLinear* tracer)
{

	WPoint a     = tracer->cursor;
	WPoint b     = a;
	tracer->prev = a;

	b.x += tracer->heading.x;
	b.y += tracer->heading.y;

	double d1 = wsh_dist2d_p(&a, &b);
	double d2 = wsh_dist2d_p(&a, &tracer->next);

	//printf("1 %f 2 %f\n", d1, d2);

	if (d2 < d1)
	{
		//printf("collide!\n");
		collide(tracer);
		return;
	}
	/*
	double adx = fabs(b.x) - fabs(a.x);
	double ady = fabs(b.y) - fabs(a.y);
	if (adx < 0 || ady < 0) {
		printf("overshot!\n");
		collide(tracer);
		return;
	}*/

	tracer->cursor = b;
	//b.x += cos
}

#define TMP_P 32

void v_WTracerLinear_draw(WTracerLinear* tracer)
{

	WPoint a = tracer->cursor;
	WPoint b = tracer->prev;

	double z = a.pressure * TMP_P;

	drw_line(a.x, a.y, b.x, b.y);
	drw_line_3f(b.x, b.y, 0, b.x, b.y, z);
	drw_line_3f(b.x, b.y, z, a.x, a.y, z);
	drw_line_3f(a.x, a.y, z, a.x, a.y, 0);
	drw_push();

	drw_translate(b.x, b.y, 0);
	//r_circle(1);
	//r_circle(64);
	drw_pop();
}
