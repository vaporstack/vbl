//
//  v_tracer.h
//  vbl
//
//  Created by vs on 2/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef v_tracer_h
#define v_tracer_h

#include <r4/src/geo/r_line.h>
#include <wsh/wsh.h>

typedef struct WTracerTime
{
	int      active;
	int      index;
	int      loop;
	double   pos;
	WPoint*  data;
	WLineHnd hnd;
} WTracerTime;

typedef struct WTracerLinear
{
	int    active;
	int    index;
	int    loop;
	double speed;
	double pos;
	WPoint cursor;
	WPoint prev;
	WPoint next;
	WPoint heading;
	RPoint dir;
	//RLine* rline;
	WLineHnd hnd;
} WTracerLinear;

WTracerLinear* v_WTracerLinear_create(WLine* data);
void	   v_WTracerLinear_destroy(WTracerLinear* tracer);
void	   v_WTracerLinear_update(WTracerLinear* tracer);
void	   v_WTracerLinear_draw(WTracerLinear* tracer);
void	   v_WTracerLinear_start(WTracerLinear* tracer);
void	   v_WTracerLinear_stop(WTracerLinear* tracer);

#endif /* v_tracer_h */
