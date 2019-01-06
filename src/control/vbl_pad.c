//
//  vbl_pad.c
//  vbl
//
//  Created by vs on 10/4/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_pad.h"
#include <stdlib.h>

#define PAD_DECREASE .999
#define PAD_CUTOFF .001

static int calc_index(VBLPad* pad, int x, int y)
{
	return (y * pad->w) + x;
}

VBLPad* vbl_pad_create(int w, int h, int ox, int oy)
{
	VBLPad* pad = calloc(1, sizeof(VBLPad));
	pad->w      = w;
	pad->h      = h;
	pad->x      = ox;
	pad->y      = oy;

	int width = w * h;
	pad->dims = width;

	pad->state  = calloc(width, sizeof(bool));
	pad->vals   = calloc(width, sizeof(float));
	pad->active = true;
	pad->cb     = NULL;

	return pad;
}

void vbl_pad_destroy(VBLPad* pad)
{
	free(pad->vals);
	free(pad->state);
	free(pad);
}

void vbl_pad_toggle(void* data, int x, int y, int v)
{
	VBLPad* pad       = (VBLPad*)data;
	int     index     = calc_index(pad, x, y);
	pad->state[index] = v;

	pad->active = true;
	printf("PaD TOGgLE\n");
}

void vbl_pad_update(VBLPad* pad)
{
	if (!pad->active)
		return;

	bool any_alive = false;
	for (int i = 0; i < pad->dims; i++)
	{
		if (pad->state[i] == 1)
		{
			pad->vals[i] = 1;
			any_alive    = true;
		}

		if (pad->vals[i] != 0)
		{
			any_alive = true;
			pad->vals[i] *= PAD_DECREASE;
			printf("%f\n", pad->vals[i]);
		}
		if (pad->vals[i] < PAD_CUTOFF)
			pad->vals[i] = 0;
	}
	pad->active = any_alive;
}

void vbl_pad_set_callback(VBLPad* pad, vbl_pad_func func)
{
	if (!func)
	{
		printf("Error, null func!\n");
		return;
	}
	pad->cb = func;
}

int vbl_pad_calc_index(VBLPad* pad, int x, int y)
{
	return (y * pad->w) + x;
}
