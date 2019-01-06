
//  v_track.c
//  v_track
//
//  Created by Andrew Macfarlane on 03/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_track_h_
#define v_track_h_

#include "v_mark.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct VSeqTrack
{
	bool      locked;
	int       mark_num;
	int       mark_pos;
	VSeqMark* marks;

} VSeqTrack;

#endif
