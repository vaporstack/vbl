
//  v_unit.c
//  v_unit
//
//  Created by Andrew Macfarlane on 30/07/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_unit.h"

VUnit* v_unit_create(void)
{
	VUnit* unit    = calloc(1, sizeof(VUnit));
	unit->position = vec3_create(NULL);

	return unit;
}

void v_unit_destroy(VUnit* unit) { free(unit); }
