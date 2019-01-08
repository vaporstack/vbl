//
//  vbl_cascade.h
//  vbl
//
//  Created by vs on 1/5/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_cascade_h
#define vbl_cascade_h


#include <vbl/vbl.h>
#include <vbl/src/vbl_types.h>

/*
enum VBLFrequenctTypes
{
	VBL_FREQUENCY_FRAME,
	VBL_FREQUENCY_SECOND,
	VBL_FREQUENCY_REUSE
};
*/

typedef void (*vbl_cascade_fire_fn)(void*);

typedef struct VCascade
{
	VBLFrequencyAttrs attrs;
	vbl_cascade_fire_fn fire;
	vbl_cascade_fire_fn update;
	void* target;
	void* target2;
	//int foo;
	//unsigned frequency_type;
	//double frequency;
}VCascade;

VCascade* vbl_cascade_create(void);
VCascade* vbl_cascade_create_attr(VBLFrequencyAttrs attr);
void vbl_cascade_destroy(VCascade* casc);
void vbl_cascade_update(VCascade* casc);


#endif /* vbl_cascade_h */
