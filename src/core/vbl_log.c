//
//  vbl_log.c
//  vbl
//
//  Created by vs on 2/8/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#include "vbl_log.h"


#include <stdarg.h>

//	todo, add getter and setter?

//extern void* _vbl_internal_rng;

#define VBL_LOG_MAX 256

void vbl_log(char* format, ...)
{
	
	char buf[VBL_LOG_MAX];
	sprintf(buf, "%s", format);
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);
	
#ifdef DEBUG
	printf("[vbl]: %s\n", buf);
#else
	printf("DO NOTHING LATER: %s\n", buf);
#endif
}



