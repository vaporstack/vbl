
//  v_unit.c
//  v_unit
//
//  Created by Andrew Macfarlane on 30/07/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_unit_h_
#define v_unit_h_

#include <vbl/src/geo/v_block.h>
typedef void (*unit_func)(VBlock*);
#include <r4/r4.h>

typedef struct VUnit
{
	int	id;
	vec3_t     position;
	unit_func* update;
	unit_func* render;
	VBlock*    data;

} VUnit;

VUnit* v_unit_create(void);
void   v_unit_destroy(VUnit*);

#endif
