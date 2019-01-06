
//  v_clock.c
//  v_clock
//
//  Created by Andrew Macfarlane on 03/08/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_clock.h"

#include <r4/r4.h>

VClock* v_clock_create(void)
{
	VClock* clock	= calloc(1, sizeof(VClock));
	clock->timesig_major = 4;
	clock->timesig_minor = 4;

	clock->loop	= 4;
	clock->lapsed      = false;
	clock->subdiv      = 4;
	clock->last	= 0;
	clock->last_sample = 0;
	clock->callback    = NULL;
	clock->position    = 0;

	return clock;
}

void v_clock_destroy(VClock* clock)
{
	free(clock);
}

void v_clock_tap(VClock* clock)
{
	clock->last_sample = r_time();
}

void v_clock_update(VClock* clock)
{
	double now = r_time();
	// aprintf("now: %f\n", now);
	double elapsed  = now - clock->last;
	clock->position = elapsed / clock->loop;
	clock->lapsed   = false;
	if (elapsed > clock->loop) {
		// printf("loop a doop %f\n", now);
		clock->last = now;
		if (clock->callback_loop)
			clock->callback_loop(clock);
		clock->lapsed = true;
	}
}
