//
//  vbl_time.c
//  vbl
//
//  Created by vs on 3/8/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_time.h"

#include <stdlib.h>

static vbl_time_func tfun = NULL;
static vbl_bool_func pfun = NULL;

#include "vbl_log.h"

void vbl_provide_time_paused_func(vbl_bool_func fun)
{
	pfun = fun;
}

void vbl_provide_time_func(vbl_time_func fun)
{
	tfun = fun;
}

double vbl_time(void)
{
	if ( !tfun )
	{
		vbl_log("Unable to query vbl time, no time function provided!");
		return -1;
	}
	return tfun();
}


int vbl_time_paused(void)
{
	if ( !pfun )
	{
		vbl_log("Unable to query vbl paused, no time function provided!");
		return -1;
	}
		
	return pfun();
}
