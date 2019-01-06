//
//  vbl_pad.h
//  vbl
//
//  Created by vs on 10/4/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_pad_h
#define vbl_pad_h

#include <stdbool.h>
#include <stdio.h>

typedef void (*vbl_pad_func)(int, int, int);

typedef struct VBLPad
{
	int	  w, h;
	int	  x, y;
	int	  dims;
	bool	 active;
	bool*	state;
	float*       vals;
	vbl_pad_func cb;
} VBLPad;

VBLPad* vbl_pad_create(int w, int h, int ox, int oy);
void    vbl_pad_destroy(VBLPad* pad);
void    vbl_pad_toggle(void* data, int x, int y, int v);
void    vbl_pad_update(VBLPad* pad);
void    vbl_pad_set_callback(VBLPad* pad, vbl_pad_func func);
int     vbl_pad_calc_index(VBLPad* pad, int x, int y);

#endif /* vbl_pad_h */
