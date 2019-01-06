
//  v_player.c
//  v_player
//
//  Created by Andrew Macfarlane on 20/06/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_player.h"
#include <stdlib.h>

SeqPlayer* v_player_create()
{
	SeqPlayer* player = calloc(1, sizeof(SeqPlayer));
	return player;
}

void v_player_destroy(SeqPlayer* seq) { seq->mode = -1; }

void v_player_play(SeqPlayer* s) { s->mode = 1; }

void v_player_pause(SeqPlayer* s) { s->mode = 0; }

void v_player_load(SeqPlayer* s, const char* path) { s->src = path; }

void v_player_set_cb(SeqPlayer* s, int track, void* fn) {}

void v_player_update(SeqPlayer* player) {}
