//
//  vbl_qhull.h
//  r4
//
//  Created by vs on 7/17/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_qhull_h
#define vbl_qhull_h

//#define R4_ENABLE_QHULL
#include <r4/src/r4_config.h>

#ifdef R4_ENABLE_QHULL

#include <r4/src/geo/r_line.h>


#include <stdio.h>

typedef struct RQHull
{
	void* src;
	int dims;
} RQHull;

void vbl_qhull_test(int num);

void vbl_qhull_draw_2d_noninvasive(RQHull* data);
void vbl_qhull_draw(RQHull* data );
void vbl_qhull_draw_fill(RQHull* data);
void vbl_qhull_draw_edges(RQHull* data);

RQHull* vbl_qhull_create_randsphere(int num);
void    vbl_qhull_destroy(RQHull*);

RQHull* vbl_qhull_create2d(RLine* line);
void 	vbl_qhull_destroy2d(RQHull* hull);

#endif

#endif /* vbl_qhull_h */
