//
//  vbl_types.h
//  vbl
//
//  Created by vs on 1/5/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_types_h
#define vbl_types_h


typedef struct VBLFrequencyAttrs
{
	unsigned type;
	double frequency;
	double last;
}VBLFrequencyAttrs;

enum VBLFrequencyDefinitions
{
	VBL_FREQUENCY_FRAME, 	//	happens on every frame or every nth frame
	VBL_FREQUENCY_FREQ,	//	happens on a seconds as double interval
	VBL_FREQUENCY_RANDOM,	//	has a random chance to happen
	VBL_FREQUENCY_NONE	//	idk?
};


#endif /* vbl_types_h */
