//
//  v_stepseq.c
//  vbl
//
//  Created by Andrew Macfarlane on 8/1/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "v_stepseq.h"

#include <stdlib.h>

#include "../core/vbl_time.h"
#include "../core/vbl_log.h"
#include <math.h>

StepSeq* v_stepseq_create(void)
{
	StepSeq* seq = calloc(1, sizeof(StepSeq));
	seq->bpm     = 120;
	// saskrotch
	//seq->bpm    = 220;
	seq->paused       = true;
	seq->subdiv       = 4;
	seq->bars	 = 4;
	seq->beats	= 4;
	seq->time	 = 0;
	seq->frac	 = 0;
	seq->width	= 4;
	seq->height       = 4;
	seq->length       = seq->bars * seq->beats;
	int num		  = seq->length;
	seq->permutations = 3;
	seq->fracs	= calloc(seq->subdiv, sizeof(double));
	seq->toggles      = calloc(seq->length, sizeof(int));
	seq->fired	= calloc(seq->length, sizeof(int));

	vbl_log("Created step sequencer with %d: %d/%d\n", num, seq->bars, seq->beats);

	return seq;
}

void v_stepseq_destroy(StepSeq* seq)
{
	free(seq->toggles);
	free(seq->fracs);
	free(seq->fired);
	free(seq);
}

void v_stepseq_reset_fireds(StepSeq* seq)
{

	//printf("Reset!\n");
	for (int i = 0; i < seq->length; i++)
		seq->fired[i] = false;
}

void v_stepseq_flip_xy(StepSeq* seq, int x, int y)
{
	int idx = (seq->width * y) + x;
	v_stepseq_flip_index(seq, idx);
}

void v_stepseq_flip_index(StepSeq* seq, int idx)
{
	seq->toggles[idx]++;
	if (seq->toggles[idx] > seq->permutations)
	{
		seq->toggles[idx] = 0;
	}
}
void v_stepseq_clear(StepSeq* seq)
{
	for (int i = 0; i < seq->length; i++)
	{
		seq->toggles[i] = 0;
	}
}

void v_stepseq_flip_at_playhead(StepSeq* seq)
{
	v_stepseq_flip_index(seq, seq->pos);
}

void v_stepseq_update(StepSeq* seq)
{
	if (seq->paused)
	{
		return;
	}

	seq->time = vbl_time();

	double now_in_seconds = fmod(seq->time, 60);
	double now_in_minutes = now_in_seconds / 60;

	double tmp = 100.0 / seq->bpm;

	double now_in_bpm = now_in_minutes * tmp * seq->bars * seq->subdiv;

	seq->frac = fmod(now_in_bpm, 1);

	for (int i = 0; i < seq->subdiv; i++)
	{
		double redux;// = (float)i / .5;
		redux	= 1. / ((i)*2);
		if (i == 0)
		{
			seq->fracs[i] = fmod(now_in_bpm, 1);
		}
		else
		{
			seq->fracs[i] = fmod(now_in_bpm * redux, 1);
		}
	}

	seq->pos = seq->frac * seq->length;

	if (seq->pos < 0)
	{
		seq->pos += seq->length;
	}
	if (seq->pos > seq->length)
	{
		//	printf("Illegal pos! hi setting to 0\n");
		seq->pos = 0;
	}
	if (seq->pos < seq->prev)
	{
		v_stepseq_reset_fireds(seq);
	}

	if (seq->pos != seq->prev)
	{
		//printf("%d\n", seq->pos);
		if (seq->toggles[seq->pos])
		{
			if (!seq->fired[seq->pos])
			{
				seq->callback(seq->pos, seq->toggles[seq->pos]);
				seq->fired[seq->pos] = true;
			}
		}
	}

	seq->prev = seq->pos;
	//for ( int i=  0; i < seq->subdiv; i++)
	//	printf("Subdivision %d is %f\n", i, seq->fracs[i] );
}

void v_stepseq_start(StepSeq* seq)
{
	seq->paused = false;
}

void v_stepseq_stop(StepSeq* seq)
{
	seq->paused = true;
}

/*
 current = LRTime::get_time();

	pos = current - last;
	pos =  pos / interval;

	double d = current-last;
	//printf("d : %f\n", d );

	if ( d > interval ){
 fire_interval();
	}




//for ( int i = 2; i <= resolution; i *=2) {
//	float tmp = pos / i;
//	int x = int(tmp * 100);
//	if ( i % x == 0 )
//		cout << i << ":" << tmp  << "/" << x << " - " << pos
//		<< endl;
//cout << tmp;
//cout << tmp << endl;
//cout << fmod(pos, tmp ) << endl;
//if ( fmod(pos, tmp ) ==0 )
//	cout << i << ":" << tmp << endl;

//}

if ( sampling ){

	if( current - last_sample > 3 ){
		printf("sample timeout.\n");
		sampling = false;

	}
}

//int main(void) {
int i = 0x80;
int ctr = 2;
float f = pos;
//for(f = 0.0f; f < 1.0f; f += 1.0f / 256.0f)
//{
i = 0x80;
ctr = 2;
for(i = 0x80, ctr = 2; ; i = i | (i >> 1), ctr *= 2) {
	if((((unsigned int)(f * 256)) & (i ^ 0xff)) == 0) {
		//printf("%f (or %d) is at timestep 1/%d \n", f, ((int)(f *
256)), ctr); fire_sequencer(ctr / 4);
	}
	if(i >= 0xfe) break;

}
///}
//	return 0;
//}

*/
