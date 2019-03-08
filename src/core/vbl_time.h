//
//  vbl_time.h
//  vbl
//
//  Created by vs on 3/8/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_time_h
#define vbl_time_h

//#include <stdbool.h>

typedef double (*vbl_time_func)(void);
typedef int (*vbl_bool_func)(void);

/*
 *
 *	this is to loosen an upwards dependency for vbl reaching up to the
 *	application layer which is not good
 *
 */
void vbl_provide_time_func(vbl_time_func);
void vbl_provide_time_paused_func(vbl_bool_func);

double vbl_time(void);
int vbl_time_paused(void);

#endif /* vbl_time_h */
