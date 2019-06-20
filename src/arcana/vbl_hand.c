//
//  vbl_hand.c
//  vbl
//
//  Created by vs on 7/2/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_hand.h"

#include <wsh/wsh.h>

#include <r4/src/ext/r_geo_interop.h>

//static bool	 init = false;
//static WDocumentHnd document;
//RLine** data = NULL;

static WObjectHnd shading;

#include <r4/r4.h>

//	todo remove this once debugging is done
//#include <drw/drw.h>
#include <r4/src/ext/r_geo_interop.h>

static void center_and_straighten(WLine* line)
{

	//wsh_line_calc_bounds(cp);
	//WRect* r = &cp->bounds;
	//double x, y;
	//x = r->pos.x;
	//y = r->pos.y;
	wsh_line_translate(line, -line->bounds.pos.x, -line->bounds.pos.y);
	wsh_line_calc_bounds(line);

	wsh_line_translate(line, line->bounds.size.x * -.5, line->bounds.size.y * -.5);
	//drw_fill_set(false);
	//drw_line(0,0, line->bounds.pos.x,line->bounds.pos.y);

	WPoint a   = line->data[0];
	WPoint b   = line->data[line->num - 1];
	double ang = wsh_angle_from_points_wp(a, b);

	//drw_line(0,0, cos(ang) * 256, sin(ang) * 256);
	wsh_line_rotate(line, 0, 0, -ang);

	//wsh_line_calc_bounds(cp);

	//drw_wline(cp);
	//wsh_line_calc_bounds(line);
	//drw_rect(line->bounds.pos.x, line->bounds.pos.y, line->bounds.pos.x + line->bounds.size.x, line->bounds.pos.y + line->bounds.size.y);

	//drw_wline(line);
}

static RLine* line_for_segment(RPoint* a, RPoint* b)
{

	unsigned long num    = shading.src->num;
	double	distab = dist2d_rp(a, b);

	double closest = INFINITY;
	//double delta = INFINITY;
	unsigned long closest_ind = 0;
	for (unsigned long i = 0; i < num; i++)
	{
		WLine* l    = shading.src->lines[i];
		double len  = wsh_line_ops_sum(l);
		double doff = distab - len;

		if (doff < closest)
		{
			closest     = len;
			closest_ind = i;
		}
	}
	//printf("closest: %lu dist: %f\n", closest_ind, closest);

	//unsigned long which = r_rand_double(NULL) * num;

	RLine* res = NULL;

	int    timeout = 500;
	int    ctr     = 0;
	bool   done    = false;
	WLine* wl      = NULL;
	do
	{
		//ctr++;
		if (ctr++ > timeout)
		{
			printf("Hit timeout! somethin's wrong\n");
			return NULL;
		}

		wl = shading.src->lines[closest_ind];
		if (wl)
			done = true;

	} while (!done);

	WLine* cp = wsh_line_copy(wl);
	center_and_straighten(cp);

	WLine* norm = wsh_line_normalize(cp, 0, 0);
	wsh_line_calc_bounds(norm);
	wsh_line_translate(norm, -norm->data[0].x, -norm->data[0].y);
	double pdx = b->x - a->x;
	double pdy = b->y - a->y;
	double ang = angle_from_points_rp(*a, *b);

	double dist = dist2d_rp(a, b);
	double dx   = norm->bounds.size.x;
	double dy   = norm->bounds.size.y;
	double ar   = dy / dx;

	//( dx > dy ) ?
	//	wsh_line_scale(norm, dx, dy):
	//	wsh_line_scale(norm, dx, dy);
	wsh_line_rotate(norm, 0, 0, ang);
	wsh_line_scale(norm, dist, dist);
	wsh_line_translate(norm, a->x, a->y);

	wsh_line_destroy(cp);
	res = r_geo_interop_rline_from_wline(norm);
	wsh_line_destroy(norm);
	return res;
}

RLine* vbl_hand_sketch(RLine* input)
{
	if (input->num > 20)
	{
		printf("Warning, this might be SLOW\n");
	}

	RLine* line = r_line_create();
	for (int i = 0; i < input->num - 1; i++)
	{
		RPoint* a	= &input->data[i];
		RPoint* b	= &input->data[i + 1];
		RLine*  addendum = line_for_segment(a, b);

		r_line_cat(line, addendum);
		//printf("Up to %lu\n", line->num);
		r_line_destroy(addendum);
	}

	return line;
}

int vbl_hand_init(const char* path)
{
	if (!path)
		path = "/Users/vs/art/barn/src/art/wash/org/components/shading/hatching_2-2018_3_4-22_44_5.wash";

	//const char * path = r_resource_load("strokes", "wash");

	if (!path)
	{
		printf("Failed to load resource!\n");
		return false;
	}

	WDocument* doc = wsh_serial_document_unserialize_file(path);
	if (!doc)
	{
		printf("Error unserializing!\n");
		return 8;
	}
	WSequence* seq = doc->sequence.src;

	WObject* obj = wsh_object_create();

	for (int i = 0; i < seq->num_frames - 1; i++)
	{
		WObject* frame = seq->frames[i];
		for (int j = 0; j < frame->num; j++)
		{
			if (frame->lines[j])
				wsh_object_add_line(obj, frame->lines[j]);
		}
	}
	printf("Concatenated %lu lines.\n", obj->num);

	//wsh_sequence_normalize_all_lines_individually(seq);
	//wsh_sequence_normalize(seq);
	shading.src = obj;

	return true;
}

void vbl_hand_deinit(void)
{
	if (shading.src)
		wsh_object_destroy(shading.src);
	//wsh_document_destroy(document.src);
}
