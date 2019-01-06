
//  v_track.c
//  v_track
//
//  Created by Andrew Macfarlane on 03/04/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_track.h"
#include "v_seq.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/*
void 	v_seq_set_track(VSeq* seq, int id)
{
	if ( id > seq->track_num)
	{
		printf("Tried to switch to a higher track than we have.\n");
		return;
	}
	seq->track_cur = id;
}

*/

VSeqTrack* v_seq_track_create()
{
	VSeqTrack* t = malloc(sizeof(VSeqTrack));
	t->locked    = false;
	t->mark_num  = 0;
	t->mark_pos  = 0;
	t->marks     = NULL;
	return t;
}

/*
void	v_seq_track_add(VSeq* seq)
{
	if ( !seq->tracks )
	{
		seq->tracks[ = v_seq_track_create();
		if(DEBUG_TRACK)
			printf("Mallocing first track.\n");
	}else{
		if ( seq->track_num < seq->track_max )
		{
			seq->tracks = realloc(seq->tracks, seq->track_num++ );
		}else{
			printf("already at track max\n");
		}
	}

	seq->track_num++;
	seq->track_cur++;
	seq->cur_track = seq->tracks[seq->track_cur-1];
}
*/

void v_seq_track_destroy(VSeq* seq, int id)
{
	// VSeqTrack* t = seq->tracks[id];

	// free(seq->tracks[id]);
}
