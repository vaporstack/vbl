//
//  vbl_rng.c
//  vbl
//
//  Created by vs on 2/8/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_rng.h"
#include <stdlib.h>

#include "vbl_log.h"

void* _vbl_internal_rng = NULL;

void vbl_rng_set(void* rng)
{

	if (rng == NULL)
	{
		vbl_log("Tried to assign a null RNG!\n");
		return;
	}
	_vbl_internal_rng = rng;
}

void* vbl_rng_get(void)
{
	return _vbl_internal_rng;
}
