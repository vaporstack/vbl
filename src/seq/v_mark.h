
//  v_mark.c
//  v_mark
//
//  Created by Andrew Macfarlane on 03/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_mark_h_
#define v_mark_h_

typedef struct VSeqMark
{
	int    id;
	double pos;
	double dur;
} VSeqMark;

VSeqMark v_seq_mark_create(void);

#endif
