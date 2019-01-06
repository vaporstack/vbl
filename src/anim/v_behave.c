
//  v_behave.c
//  v_behave
//
//  Created by Andrew Macfarlane on 11/20/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_behave.h"

#include <stdlib.h>

VBehave* v_behave_create(void)
{
	VBehave* rec  = calloc(1, sizeof(VBehave));
	rec->move     = NULL;
	rec->test     = NULL;
	rec->start    = NULL;
	rec->end      = NULL;
	rec->distance = 128;
	return rec;
}

void v_behave_destroy(VBehave* rec)
{
	free(rec);
}
