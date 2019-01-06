//
//  v_stepseq.h
//  vbl
//
//  Created by Andrew Macfarlane on 8/1/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_stepseq_h
#define v_stepseq_h

#include <stdbool.h>

typedef void(step_callback)(int bar, int beat);

typedef struct StepSeq
{

	//	the master playhead
	double time;

	bool paused;
	int  bpm;
	int  subdiv;
	int  bars;
	int  beats;

	int width;
	int height;

	int permutations;

	int pos;
	int prev;
	//	minimal history
	//int* lasts;
	//int* positions;
	double  frac;
	double* fracs;

	int*	   fired;
	int*	   toggles;
	int	    length;
	step_callback* callback;
} StepSeq;

StepSeq* v_stepseq_create(void);
void     v_stepseq_start(StepSeq* seq);
void     v_stepseq_stop(StepSeq* seq);
void     v_stepseq_destroy(StepSeq* seq);
void     v_stepseq_update(StepSeq* seq);
void     v_stepseq_tap(StepSeq* seq);
void     v_stepseq_flip_xy(StepSeq* seq, int x, int y);
void     v_stepseq_flip_index(StepSeq* seq, int idx);
void     v_stepseq_flip_at_playhead(StepSeq* seq);
void     v_stepseq_clear(StepSeq* seq);

#endif /* v_stepseq_h */
