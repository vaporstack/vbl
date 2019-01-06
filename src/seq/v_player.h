
//  v_player.c
//  v_player
//
//  Created by Andrew Macfarlane on 20/06/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_player_h_
#define v_player_h_

typedef struct SeqPlayer
{
	int	 mode;
	const char* src;
	void**      callbacks;
} SeqPlayer;

SeqPlayer* v_player_create(void);
void       v_player_destroy(SeqPlayer*);

void v_player_play(SeqPlayer* s);
void v_player_pause(SeqPlayer* s);
void v_player_load(SeqPlayer* s, const char* path);
void v_player_set_cb(SeqPlayer* s, int track, void* fn);

void v_player_update(SeqPlayer* s);

#endif
