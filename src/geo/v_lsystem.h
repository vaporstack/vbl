//
//  v_lsystem.h
//  vbl
//
//  Created by Andrew Macfarlane on 10/18/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_lsystem_h
#define v_lsystem_h

#include <stdio.h>

typedef struct LSystem
{
	const char* rules;
	int	 num;
	double      span;
} LSystem;

LSystem* v_lsystem_create(void);
void     v_lsystem_destroy(LSystem* sys);

void v_lsystem_setrules(LSystem* sys, const char* rules);

#endif /* v_lsystem_h */
