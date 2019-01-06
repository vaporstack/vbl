
//  v_tesselate.c
//  v_tesselate
//
//  Created by Andrew Macfarlane on 08/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_tesselate_h_
#define v_tesselate_h_

/** @file v_tesselate.h
 *  @brief crude module for simple (regular) tesselations
 *
 *  potentially dangerous because it's a sparse array of points, I guess
 *  with low size / many sides could get heavy real quick
 *  @author Andrew Macfarlane (_ruminant on twitter dawt cawm)
 *  @bug All the bugs.
 */

typedef struct TPoint2
{
	int    id;
	double x;
	double y;
} TPoint2;

typedef struct TFace
{
	int	   sides; // track how much we have
	struct TFace *edges; // edge is a link to a subface
	TPoint2 **    verts; // verts are the baked point coords
	double	size;  // radius
} TFace;

typedef struct TVert
{
	TFace *faces;
	int    num;

} TVert;

typedef struct VTesselate
{
	double		   size;
	int		   sides;
	int		   tilings;
	unsigned long long max_length;
	// struct TFace *     root;
	// void *		   data;   // idk what this is yet haha
	int    length; //	danger will robinoh fuck it
	int    depth;
	TVert *root;

} VTesselate;

TFace *v_face_create(VTesselate *tess);

VTesselate *v_tesselate_create(int sides, int tilings, double size);
void	v_tesselate_destroy(VTesselate *);

void v_tesselate_populate(VTesselate *tess, int depth);
void v_tesselate_find_nearest(VTesselate *tess, double x, double y);

#endif
