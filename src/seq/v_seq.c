//
//  monome.c
//  vblit
//
//  Created by Andrew Macfarlane on 1/11/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "v_seq.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v_mark.h"
#include "v_track.h"

#include "../core/vbl_time.h"
#include <assert.h>


VSeq* v_seq_create(void)
{

	VSeq* seq      = calloc(1, sizeof(VSeq));
	seq->track_num = 0;
	seq->track_cur = 0;
	seq->track_max = VBL_SEQ_MAX_TRACKS;
	seq->pos       = 0.0;
	seq->tracks    = calloc(VBL_SEQ_MAX_TRACKS, sizeof(VSeqTrack*));
	for (int i = 0; i < seq->track_num; ++i)
	{
		seq->tracks[i] = NULL;
	}

	char buf[128];
	sprintf(buf, "%d.%d.%d", VBL_V_MAJOR, VBL_V_MINOR, VBL_V_POINT);
	seq->version   = buf;
	seq->recording = false;
	seq->mark_cb	= NULL;
	seq->finish_cb = NULL;
	return seq;
}


void v_seq_set_playback_cb(VSeq* seq, vseq_playback_cb cb)
{
	seq->mark_cb = cb;
}

void v_seq_set_finish_cb(VSeq* seq, vseq_playback_finish cb)
{
	seq->finish_cb = cb;
}

void v_seq_set_recording(VSeq* seq, int v)
{
	seq->recording = v;
}

void v_seq_track_clear(VSeqTrack* track)
{
	free(track->marks);
	track->marks    = NULL;
	track->mark_num = 0;
	track->mark_pos = 0;
}

void v_seq_start(VSeq* seq)
{

	seq->pos	= 0.0;
	seq->time_start = vbl_time();
}

void v_seq_stop(VSeq* seq)
{
	printf("This does nothing yet!\n");
}

static void check_track_for_unposted(VSeq* seq, VSeqTrack* track, int track_id)
{
	//		m_pos                           ->|
	//              |
	// -------------==----=-----=--------===-------====-------
	//
	

	
	for (int i = track->mark_pos; i < track->mark_num; i++)
	{
		VSeqMark* m = &(track->marks[i]);
		if ( !m )
			continue;
		
		if (m->pos > seq->pos)
			break;
		
		track->mark_pos = m->id+1;
		
		seq->mark_cb(track_id, m);
	}
}

static void check_tracks_for_unposted_markers(VSeq* seq)
{
	for (int i = 0; i < seq->track_num; i++)
	{
		VSeqTrack* track = seq->tracks[i];
		if ( !track)
			continue;
		if ( track->marks )
			check_track_for_unposted(seq, track, i);
	}
}

void v_seq_update(VSeq* seq)
{
	double time   = vbl_time();
	double ntime  = time / seq->time_duration;
	seq->pos      = ntime;
	seq->time_cur = time;
	
	if (seq->time_start + seq->time_cur > seq->time_duration)
	{
		//	probably should do other stuff here like update the playback state
		printf("DONE\n");
		if ( seq->finish_cb)
			seq->finish_cb();
		
	}
	
	if (seq->recording)
		return;

	check_tracks_for_unposted_markers(seq);
}

void v_seq_destroy(VSeq* seq)
{
	for (int i = 0; i < seq->track_num; ++i)
	{
		v_seq_track_destroy(seq, i);
	}
	free(seq);
}

void v_seq_unmark(struct VSeq* seq)
{

	VSeqTrack* track = seq->cur_track;
	if (!track)
	{
		printf("Error, no track! can't unmark\n");
		return;
	}

	VSeqMark* m = &track->marks[track->mark_num - 1];
	m->dur      = seq->pos - m->pos;
	printf("dur: %f\n", m->dur);
}

void v_seq_mark(struct VSeq* seq)
{

	int	id    = seq->track_cur;
	VSeqTrack* track = seq->tracks[id];

	if (track == NULL)
	{
		track		= v_seq_track_create();
		seq->tracks[id] = track;
		seq->cur_track  = track;
		seq->track_num++;
	}

	track->locked = true;
	track->mark_num++;

	if (track->marks == NULL)
	{
		track->marks = malloc(sizeof(VSeqMark));
	}
	else
	{
		track->marks = realloc(track->marks,
				       sizeof(VSeqMark) * track->mark_num + 1);
	}

	VSeqMark m;
	// VSeqMark* m = v_seq_mark_create();
	m.id				  = track->mark_num - 1;
	m.pos				  = seq->pos;
	track->marks[track->mark_num - 1] = m;
	printf("marking! %f %d\n", m.pos, track->mark_num);

	track->locked = false;
}

long v_seq_count_marks(VSeq* seq)
{
	int acc = 0;
	for ( int i = 0;i < seq->track_num; i++ )
	{
		VSeqTrack*track = seq->tracks[i];
		if (!track)
			continue;
		
		acc += track->mark_num;
		
	}
	return acc;
}


long v_seq_count_marks_track(VSeq* seq, int index)
{
	
	assert(index < seq->track_num);
	VSeqTrack* track = seq->tracks[index];
	if ( !track)
		return -1;
	
	return track->mark_num;

}

int v_seq_get_track_count(VSeq* seq)
{
	return seq->track_num;
}

void v_seq_time_set_start(VSeq* seq)
{
	seq->pos = 0;
}

void v_seq_time_set_end(VSeq* seq)
{
	seq->pos = seq->time_duration;
}

void v_seq_playback_start(VSeq* seq)
{
	// seq->time_start = vbl_time();
	printf("Started sequence play at %f\n", seq->time_start);
}

void v_seq_track_switch(VSeq* seq, int id)
{

	if (id >= seq->track_max)
	{
		printf("NOP\n");
		id = 0;
	}
	if (id < 0)
	{
		id = seq->track_max - 1;
	}

	printf("Switch to track %d\n", id);
	seq->track_cur = id;
	seq->cur_track = seq->tracks[id];
}
/*
void	v_seq_serialize(VSeq* v, const char* path)
{

	//



}

VSeq*	v_seq_unserialize(char* path )
{

	return NULL;


}
*/
