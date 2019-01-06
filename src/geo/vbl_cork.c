//
//  vbl_cork.c
//  r4
//
//  Created by vs on 7/17/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_cork.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <deps/cork/cork.h>

#define NTRIS 12
#define NVERTS 8

// 	box.off
//	from http://people.sc.fsu.edu/%7Ejburkardt/data/off/off.html

/*
 OFF
 8 6 0
 -0.500000 -0.500000 0.500000
 0.500000 -0.500000 0.500000
 -0.500000 0.500000 0.500000
 0.500000 0.500000 0.500000
 -0.500000 0.500000 -0.500000
 0.500000 0.500000 -0.500000
 -0.500000 -0.500000 -0.500000
 0.500000 -0.500000 -0.500000
 4 0 1 3 2
 4 2 3 5 4
 4 4 5 7 6
 4 6 7 1 0
 4 1 7 5 3
 4 6 0 2 4
 */

RCorkRec* vbl_cork_load(const char* file)
{

	struct CorkTriMesh* mesh = calloc(1, sizeof(struct CorkTriMesh));

	mesh->n_triangles = 24;
	mesh->n_vertices  = 24;
	mesh->triangles   = calloc(mesh->n_triangles * 3, (sizeof(uint)));
	mesh->vertices    = calloc(mesh->n_vertices * 3, (sizeof(float)));

	const float vertdata[24] = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0};
	const int   tridata[24]  = {0, 1, 2, 3, 4, 5, 6, 7, 2, 3, 7, 6, 0, 1, 5, 4, 0, 4, 7, 3, 1, 2, 6, 5};
	//	this data is from box.off but flattened

	//const int tridata[8*3] = {0, 1, 3, 2, 2, 3, 5, 4, 4, 5, 7, 6, 6, 7, 1, 0, 1, 7, 5, 3, 6, 0, 2, 4};
	//const float vertdata[8*3] = {-0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5};

	for (uint i = 0; i < mesh->n_triangles; i++)
	{
		mesh->triangles[i] = tridata[i];
	}

	for (uint i = 0; i < mesh->n_vertices; i++)
	{
		mesh->vertices[i] = vertdata[i];
	}

	bool is_solid = isSolid(*mesh);

	printf("Cork is solid? %d\n", is_solid);

	(is_solid) ? printf("yes\n") : printf("no\n");

	//	yes
	RCorkRec* rec = calloc(1, sizeof(RCorkRec));
	rec->src      = mesh;
	return rec;
}
