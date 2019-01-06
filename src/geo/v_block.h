//
//  r_vec.h
//  r4
//
//  Created by Andrew Macfarlane on 1/7/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_block_h_
#define v_block_h_

//#include "r_vec.h"

#include <deps/gl-matrix/gl-matrix.h>

//	TODO? this is used by grdn, now, so
//	maybe it should be moved back into r core?

#include "v_vec.h"
typedef vec3_t*  vec3_list;
typedef vec3_t** vec3_list2;

typedef struct VBlock
{
	// int dims;
	int		   width;
	VVec**		   data;
	VVec**		   data2;
	VVec**		   extra;
	unsigned long long len;
} VBlock;

VBlock* v_block_create(int);
void    v_block_destroy(VBlock*);

VVec* v_block_get(VBlock* block, int x, int y, int z);

VVec* v_block_get_data(VBlock*, int, int, int);
VVec* v_block_get_data2(VBlock*, int, int, int);
VVec* v_block_get_extra(VBlock*, int, int, int);

void v_block_set_data(VBlock* block, int x, int y, int z, vec3_t v);
void v_block_set_data2(VBlock* block, int x, int y, int z, vec3_t v);
void v_block_set_extra(VBlock* block, int x, int y, int z, vec3_t v);
void v_block_set(VBlock*, int, int, int, double, double, double);
void v_block_setdata2(VBlock*, int, int, int, double, double, double);
void v_block_setextra(VBlock*, int, int, int, double, double, double);
void v_block_set_vec(VBlock*, int, int, int, vec3_t*);
void v_block_set_index(VBlock*, int, double, double, double);
void v_block_set_index_vec(VBlock*, int, vec3_t*);

void v_block_test(void);
void v_block_dist_grid(VBlock*, double, double);
void v_block_dist_rand(VBlock*, double, double);

void v_block_dist_sphere(VBlock*, double);
void v_block_dist_sphere_vol(VBlock*, double);

int v_block_offset(VBlock*, int, int, int);

#endif /* v_block_h_ */
