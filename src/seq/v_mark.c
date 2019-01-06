
//  v_mark.c
//  v_mark
//
//  Created by Andrew Macfarlane on 03/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_mark.h"

#include <stdio.h>

/*
typedef struct VSeqMark
{
	int id;
	double pos;
	double duration;
} VSeqMark;

 */

VSeqMark v_seq_mark_create()
{
	VSeqMark m; // = malloc(sizeof(VSeqMark));
	m.id  = -1;
	m.pos = -1;
	m.dur = -1;

	return m;
}
