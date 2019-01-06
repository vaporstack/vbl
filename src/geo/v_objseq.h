
//  v_objseq.c
//  v_objseq
//
//  Created by Andrew Macfarlane on 02/22/18.
//  Copyright © 2018 vaporstack. All rights reserved.

#ifndef v_objseq_h_
#define v_objseq_h_

//#include <drw/drw.h>
//#include <r4/src/res/r_assimp.h>

#include <r4/src/r4_config.h>

#ifdef R4_ENABLE_ASSIMP

#include <r4/src/res/r_assimp.h>

typedef struct VObjSeq
{
	const char*	 basepath;
	struct DAssimpRec** data;
	int		    num;
	bool		    loaded;

} VObjSeq;

VObjSeq* v_objseq_create(const char* path, int defer_load);
void     v_objseq_load(VObjSeq* seq);
void     v_objseq_destroy(VObjSeq* seq);

#endif

#endif
