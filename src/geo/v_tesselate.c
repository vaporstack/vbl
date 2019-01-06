
//  v_tesselate.c
//  v_tesselate
//
//  Created by Andrew Macfarlane on 08/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_tesselate.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//	todo fix this, we're including it just for M_PI
#include <r4/src/core/r_math.h>

/*

 static inline double
 dist2d(double x1, double y1, double x2, double y2)
 {
	return sqrt((fabs(x1 - x2)) * (fabs(x1 - x2)) +
 (fabs(y1 - y2)) * (fabs(y1 - y2)));
 };
 */
static void _tess_generate(TVert* face, int max, int* depth);

void _tess_generate(TVert* vert, int max, int* depth)
{
	printf("Generating tess at depth %d %d?\n", max, *depth);

	if (*depth >= max) {
		printf("done!\n");
		return;
	}
	(*depth) += 1;
	printf("depth %d\n", *depth);
	//_tess_generate(face, max, depth);
}

TVert* v_vert_create(void)
{
	TVert* v = calloc(1, sizeof(TVert));
	v->num   = 0;
	return v;
}

VTesselate* v_tesselate_create(int sides, int tilings, double size)
{
	VTesselate* tess = calloc(1, sizeof(VTesselate));
	tess->sides      = sides;
	tess->size       = size;
	tess->root       = v_vert_create();
	tess->depth      = 0;
	tess->tilings    = tilings;

	return tess;
}

TFace* v_face_create(VTesselate* tess)
{
	TFace* face = calloc(1, sizeof(TFace));
	face->sides = tess->sides;
	face->size  = tess->sides;

	face->verts = calloc(tess->sides, sizeof(TPoint2**));
	int    num  = tess->sides;
	double inc  = 2 * M_PI / ((float)num);

	for (int j = 0; j < tess->tilings; j++) {

		for (int i = 0; i < tess->sides; i++) {
			double degi = inc * i;

			double x       = cos(degi) * tess->size;
			double y       = sin(degi) * tess->size;
			face->verts[i] = calloc(1, sizeof(TPoint2));

			face->verts[i]->x = x;
			face->verts[i]->y = y;
		}
	}
	return face;
}

void v_tesselate_populate(VTesselate* tess, int max)
{

	int depth = 0;
	printf("Generating data to depth %d %d\n", max, depth);
	TVert* root = tess->root;
	if (root == NULL) {
		// root       = v_face_create(tess);
		tess->root = root;
	}

	_tess_generate(root, max, &depth);
	tess->depth = max;
}

void v_tesselate_find_nearest(VTesselate* tess, double x, double y)
{
	/*
	 double radius = tess->size;
	 //	loop through every point and find any points that are within the
	 //	radius ( I think this is guaranteed to find at least two)
	 for (int i = 0; i < tess->length; i++)
	 {
		if (tess->data == NULL)
		{
	 printf("reallocing to fuckit I'm mad\n");
		}
	 }
	 */
}

void v_tesselate_destroy(VTesselate* tess) { free(tess); }
