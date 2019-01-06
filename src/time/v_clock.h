
//  v_clock.c
//  v_clock
//
//  Created by Andrew Macfarlane on 03/08/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_clock_h_
#define v_clock_h_

#include <stdbool.h>

typedef void(clock_callback)(int bar, int beat);
typedef void(clock_loop_callback)(void* clock);

#define CLOCK_SAMPLES 16

typedef struct VClock
{
	bool   paused;
	bool   lapsed;
	double last;
	double last_sample;
	double position;
	int    resolution;

	int loop;
	int subdiv;

	int timesig_major;
	int timesig_minor;

	clock_callback*      callback;
	clock_loop_callback* callback_loop;
	double		     samples[CLOCK_SAMPLES];
} VClock;

VClock* v_clock_create(void);
void v_clock_destroy(VClock* clock);
void v_clock_tap(VClock* clock);
void v_clock_update(VClock* clock);
void v_clock_snap(VClock* clock);

#endif
