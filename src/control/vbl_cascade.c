//
//  vbl_cascade.c
//  vbl
//
//  Created by vs on 1/5/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_cascade.h"

#include <r4/src/core/r_random.h>
#include <vbl/vbl.h>

VCascade* vbl_cascade_create(void)
{
	VCascade* csc = calloc(1, sizeof(VCascade));
	return csc;
}


VCascade* vbl_cascade_create_attr(VBLFrequencyAttrs attr)
{
	VCascade* csc = vbl_cascade_create();
	csc->attrs = attr;
	return csc;
}


void vbl_cascade_destroy(VCascade* casc)
{
	free(casc);
}

static void fire_safely(VCascade* casc)
{
	if (!casc->fire)
	{
		vbl_log("Can't fire, no fire function defined %s\n", __func__);
		return;
	}
	if ( !casc->target)
	{
		printf("No target defined!\n");
		return;
	}
	casc->fire(casc);
}

static void update_frame(VCascade* casc)
{
	//	probably nothing could go wrong treating doubles as integers here?
	//	famous last words TODO lol
	casc->attrs.last++;
	if ( casc->attrs.last > casc->attrs.frequency )
	{
		fire_safely(casc);
		casc->attrs.last = 0;
	}
}

static void update_freq(VCascade* casc)
{
	double now = r_time();
	double elapsed = now - casc->attrs.last;
	if ( elapsed > casc->attrs.frequency )
	{
		fire_safely(casc);
		casc->attrs.last = now;
	}
}

static void update_random(VCascade* casc)
{
	double throw = casc->attrs.frequency;
	RRandom* rng = vbl_rng_get();
	if ( r_rand_double(rng) < throw )
	{
		fire_safely(casc);
	}
}

void vbl_cascade_update(VCascade* casc)
{
	//	do something
	if(!casc->target )
	{
		printf("Can't fire, no target provided.\n");
		return;
	}
	switch (casc->attrs.type) {
		case VBL_FREQUENCY_FRAME:
			update_frame(casc);
			break;

		case VBL_FREQUENCY_FREQ:
			update_freq(casc);
			break;
			
		case VBL_FREQUENCY_RANDOM:
			update_random(casc);
			break;
			
		case VBL_FREQUENCY_NONE:
			printf("Undefined frequency update type.\n");
			break;
		default:
			break;
	}
	
}

