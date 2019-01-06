//
//  monome.h
//  vblit
//
//  Created by Andrew Macfarlane on 1/11/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_seq_h_
#define v_seq_h_

#include <stdbool.h>

#include "v_mark.h"
#include "v_track.h"

#define VBL_V_MAJOR 0
#define VBL_V_MINOR 0
#define VBL_V_POINT 1

#define VBL_SEQ_MAX_TRACKS 8

typedef void vseq_playback_cb( int, VSeqMark* m );
typedef void vseq_playback_finish( void) ;

typedef struct VSeq
{
	int playback_state;
	int recording;

	int track_cur;
	int track_num;
	int track_max;

	//	times, in normal human seconds
	double time_start;
	double time_cur;
	double time_duration;

	//	position, normalized 0-1
	double pos;

	const char* ref;
	const char* version;

	VSeqTrack*  cur_track;
	VSeqTrack** tracks;

	vseq_playback_finish* finish_cb;
	vseq_playback_cb* mark_cb;
} VSeq;


VSeq* v_seq_create(void);
void  v_seq_set_recording(VSeq* seq, int val);
void  v_seq_destroy(VSeq*);
void  v_seq_mark(VSeq*);
void  v_seq_unmark(VSeq*);

long v_seq_count_marks(VSeq* seq);
long v_seq_count_marks_track(VSeq* seq, int i);
int v_seq_get_track_count(VSeq* seq);
void v_seq_time_set_start(VSeq* seq);
void v_seq_time_set_end(VSeq* seq);
void v_seq_update(VSeq*);

void v_seq_playback_start(VSeq* seq);

void v_seq_set_playback_cb(VSeq* seq, vseq_playback_cb cb);
void v_seq_set_finish_cb(VSeq* seq, vseq_playback_finish cb);
void v_seq_start(VSeq* seq);
void v_seq_stop(VSeq* seq);

VSeqTrack* v_seq_track_create(void);
void       v_seq_track_add(VSeq* seq);
void       v_seq_track_destroy(VSeq* seq, int);
void       v_seq_track_clear(VSeqTrack* track);

void v_seq_track_switch(VSeq*, int);
void v_seq_mark_add(VSeq*);

void  v_seq_serialize(VSeq*, const char* path);
VSeq* v_seq_unserialize(const char* path);

#endif /* sequencer_h_ */
