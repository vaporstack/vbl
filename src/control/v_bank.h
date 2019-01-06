
//  v_bank.c
//  v_bank
//
//  Created by Andrew Macfarlane on 02/22/18.
//  Copyright © 2018 vaporstack. All rights reserved.

#ifndef v_bank_h_
#define v_bank_h_

#include <stdbool.h>

typedef struct VBank
{
	int   id;
	float intensity;
	int   overdrive;
	bool  flipx;
	bool  flipy;

} VBank;

#endif
