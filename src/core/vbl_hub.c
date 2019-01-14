//
//  vbl_hub->c
//  vbl
//
//  Created by vs on 4/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_hub.h"

#include <cjson/cJSON.h>
#include <drw/drw.h>
//insufficient on ios, luulz
//	#include <r4/src/r4_config.h>
#include <r4/r4.h>
//#include <r4/r4.h>

#include "../util/v_serial.h"

//#include "../vblit_common.h"
//#include "../generators/v_show.h"

/*
 void vbl_m128_bank_toggle_flipy(void* mdata, int x, int y, int v)
 {
 if (hub->bank == -1)
 {
 return;
 }
 VblBank* b = &hub->banks[hub->bank];
 b->flipy     = !b->flipy;
 printf("[%d] flipy set to %d\n", hub->bank, hub->banks[hub->bank].flipx);
 }
 */

void vbl_hub_toggle_bank_color(VblHub* hub)
{
	if (hub->bank == -1)
	{
		return;
	}
	VblBank* b = &hub->banks[hub->bank];
	b->color   = !b->color;
	printf("[%d] color set to %d\n", hub->bank, hub->banks[hub->bank].color);
}

void vbl_hub_toggle_bank_colorcycle(VblHub* hub)
{
	if (hub->bank == -1)
	{
		return;
	}
	VblBank* b = &hub->banks[hub->bank];
	b->colorcycle++;
	if (b->colorcycle >= VBL_COLORCYCLE_END)
		b->colorcycle = 0;

	//= !b->fill;
	printf("[%d] colorcycle set to %d\n", hub->bank, hub->banks[hub->bank].colorcycle);
}

void vbl_hub_toggle_bank_fill(VblHub* hub)
{
	if (hub->bank == -1)
	{
		return;
	}
	VblBank* b = &hub->banks[hub->bank];
	b->fill    = !b->fill;
	printf("[%d] fill set to %d\n", hub->bank, hub->banks[hub->bank].fill);
}

void vbl_hub_toggle_bank_flipx(VblHub* hub)
{
	if (hub->bank == -1)
	{
		return;
	}
	VblBank* b = &hub->banks[hub->bank];
	b->flipx   = !b->flipx;
	printf("[%d] flipy set to %d\n", hub->bank, hub->banks[hub->bank].flipx);
}

void vbl_hub_toggle_bank_flipy(VblHub* hub)
{
	if (hub->bank == -1)
	{
		return;
	}
	VblBank* b = &hub->banks[hub->bank];
	b->flipy   = !b->flipy;
	printf("[%d] flipy set to %d\n", hub->bank, hub->banks[hub->bank].flipx);
}

void vbl_hub_reset(VblHub* hub)
{
	//printf("reset!\n");
	//hub->debug = false;
	r_app_set_debug(hub->debug);
	hub->page = 0;
	hub->bank = -1;
	for (int i = 0; i < VBL_NUM_BANKS; i++)
	{
		hub->banks[i].id	 = i;
		hub->banks[i].routine    = -1;
		hub->banks[i].flipx      = false;
		hub->banks[i].flipy      = false;
		hub->banks[i].intensity  = 1;
		hub->banks[i].fill       = 0;
		hub->banks[i].color      = 0;
		hub->banks[i].colorcycle = VBL_COLORCYCLE_NONE;
		hub->assignments[i]      = -1;
	}

	//if(sequencer)
	//	v_stepseq_clear(sequencer);
	hub->playback_speed = 1;
}

void vbl_hub_sequencer_reset(VblHub* hub)
{
	//	if ( sequencer)
	//		v_stepseq_clear(sequencer);
}

void vbl_hub_toggle_overdrive(VblHub* hub)
{
	//	hub->overdrive = !hub->overdrive;
	//	(hub->overdrive) ? 	printf("OVERDRIVE\n"): printf("NOVERDRIVE\n");
}

void vbl_hub_toggle_auto(VblHub* hub)
{
	//vbl_control_auto_toggles = ! vbl_control_auto_toggles;
	//(vbl_control_auto_toggles) ? 	printf("AUTO\n"): printf("NO AUTO\n");
}

void vbl_hub_switch_artist(VblHub* hub)
{
	hub->artist++;
	if (hub->artist > 3)
	{
		hub->artist = 0;
	}
#ifndef RPLATFORM_IOS
	v_show_hax_switch_artist_iface(hub->artist);
#endif
}

void vbl_hub_flip_sequencer_at_xy(VblHub* hub, int x, int y)
{
	//v_stepseq_flip_xy(sequencer, x, y);
}

void vbl_hub_flip_sequencer_at_current_playhead(VblHub* hub)
{
	//v_stepseq_flip_at_playhead(sequencer);
}
/*

void vbl_hub_adjust_playback(double v )
{
	hub->playback_speed =

I}
*/
void vbl_hub_toggle_mode3d(VblHub* hub)
{
	printf("MODE3d\n");
	hub->mode3d = !hub->mode3d;
	//_update_display_mode3d((MonomeRec*)mdata);

	r_app_set_ortho(hub->mode3d);
}

void vbl_hub_fire_hit(VblHub* hub, int which, int val)
{
	//printf("Doing hit for hit %d\n", val);

	int* hits_type_lut[4] = {&hub->hit1, &hub->hit2, &hub->hit3, &hub->hit4};

	//	this is almost DEFINITELY not right
	//	I think?  having a hard time visualising n*n*n dimensions lol
	//printf("Switching hit %d to type %d\n", val, val);
	*hits_type_lut[val] = val;

	//ub.hit
}

static void _unserialize_bank(cJSON* jsb, VblBank* b)
{

	cJSON* v = NULL;

	v = cJSON_GetObjectItem(jsb, "id");
	if (v)
		b->id = v->valueint;
	v = cJSON_GetObjectItem(jsb, "routine");
	if (v)
		b->routine = v->valueint;
	v = cJSON_GetObjectItem(jsb, "flipx");
	if (v)
		b->flipx = v->valueint;
	v = cJSON_GetObjectItem(jsb, "flipy");
	if (v)
		b->flipy = v->valueint;
	v = cJSON_GetObjectItem(jsb, "color");
	if (v)
		b->color = v->valueint;
	v = cJSON_GetObjectItem(jsb, "colorcycle");
	if (v)
		b->colorcycle = v->valueint;
	v = cJSON_GetObjectItem(jsb, "intensity");
	if (v)
		b->intensity = v->valueint;
	v = cJSON_GetObjectItem(jsb, "fill");
	if (v)
		b->fill = v->valueint;
	v = cJSON_GetObjectItem(jsb, "extra");
	if (v)
		b->extra = v->valueint;
	v = cJSON_GetObjectItem(jsb, "bang");
	if (v)
		b->extra = v->valueint;
}

static cJSON* _serialize_bank(VblBank* b)
{
	cJSON* jsb = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsb, "id", b->id);
	cJSON_AddNumberToObject(jsb, "routine", b->routine);
	cJSON_AddNumberToObject(jsb, "flipx", b->flipx);
	cJSON_AddNumberToObject(jsb, "flipy", b->flipy);
	cJSON_AddNumberToObject(jsb, "intensity", b->intensity);
	cJSON_AddNumberToObject(jsb, "color", b->color);
	cJSON_AddNumberToObject(jsb, "colorcycle", b->colorcycle);
	cJSON_AddNumberToObject(jsb, "fill", b->fill);
	cJSON_AddNumberToObject(jsb, "extra", b->extra);
	cJSON_AddNumberToObject(jsb, "bang", b->bang);

	return jsb;
}

static char* state_path(void)
{
	const char* path = r_get_path_prefs();
	char*       buf  = calloc(PATH_MAX, sizeof(char));
	sprintf(buf, "%s%s%s", path, R4_PATH_SEP, "vbl-state.json");
	return buf;
}

void vbl_hub_init(VblHub* hub)
{
	hub->artist      = 0;
	hub->overdrive   = 0;
	const char* path = state_path();
	if (r_file_exists(path))
	{
		vbl_hub_unserialize(hub, path);
	}

	//show_hax_switch_artist(hub->artist);
}

void vbl_hub_deinit(VblHub* hub)
{

	const char* path = state_path();
	vbl_hub_serialize(hub, path);
}

void vbl_hub_unserialize(VblHub* hub, const char* path)
{

	hub->speed      = 1;
	hub->speed_mult = 1;
	char* data      = vbl_read_file_as_text_nc(path);

	//char* buf = calloc(PATH_MAX, sizeof(char));

	if (!data)
	{
		//char buf[256];
		//sprintf(buf, "An error occurred reading text file: %s\n", path);
		l_warning("An error occurred reading text file: %s\n", path);
		//free(data);
		//free(buf);
		return;
	}

	cJSON* root = cJSON_Parse(data);

	cJSON* itr = cJSON_GetObjectItem(root, "page");
	if (itr)
		hub->page = itr->valueint;

	itr = cJSON_GetObjectItem(root, "bank");
	if (itr)
		hub->bank = itr->valueint;

	itr = cJSON_GetObjectItem(root, "playback_speed");
	if (itr)
		hub->playback_speed = itr->valuedouble;

	itr = cJSON_GetObjectItem(root, "artist");
	if (itr)
		hub->artist = itr->valueint;
	itr = cJSON_GetObjectItem(root, "overdrive");
	if (itr)
		hub->overdrive = itr->valueint;

	itr = cJSON_GetObjectItem(root, "audio");
	if (itr)
		hub->audio = itr->valueint;

	itr = cJSON_GetObjectItem(root, "debug");
	if (itr)
		hub->bank = itr->valueint;

	itr = cJSON_GetObjectItem(root, "mode3d");
	if (itr)
		hub->mode3d = itr->valueint;
	r_app_set_ortho(hub->mode3d);

	itr = cJSON_GetObjectItem(root, "data1");
	if (itr)
		hub->wave1 = itr->valueint;

	itr = cJSON_GetObjectItem(root, "data2");
	if (itr)
		hub->wave2 = itr->valueint;

	itr = cJSON_GetObjectItem(root, "data3");
	if (itr)
		hub->wave3 = itr->valueint;

	itr = cJSON_GetObjectItem(root, "data4");
	if (itr)
		hub->wave4 = itr->valueint;

	cJSON* jsbanks = cJSON_GetObjectItem(root, "banks");

	for (int i = 0; i < VBL_NUM_BANKS; i++)
	{
		cJSON* jsv = cJSON_GetArrayItem(jsbanks, i);
		_unserialize_bank(jsv, &hub->banks[i]);
	}

	cJSON* assigs = cJSON_GetObjectItem(root, "assignments");
	int    num    = cJSON_GetArraySize(assigs);

	for (int i = 0; i < num; i++)
	{
		cJSON* v = cJSON_GetArrayItem(assigs, i);
		if (v)
			hub->assignments[i] = v->valueint;
	}

	free(data);
	//free(buf);

	cJSON_Delete(root);
}

void vbl_hub_serialize(VblHub* hub, const char* path)
{
	cJSON* root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "artist", hub->artist);
	cJSON_AddNumberToObject(root, "overdrive", hub->overdrive);

	cJSON_AddNumberToObject(root, "page", hub->page);
	cJSON_AddNumberToObject(root, "bank", hub->bank);
	cJSON_AddNumberToObject(root, "audio", hub->audio);
	cJSON_AddNumberToObject(root, "debug", hub->debug);
	cJSON_AddNumberToObject(root, "mode3d", hub->mode3d);
	cJSON_AddNumberToObject(root, "playback_speed", hub->playback_speed);

	cJSON_AddNumberToObject(root, "wave1", hub->wave1);
	cJSON_AddNumberToObject(root, "wave2", hub->wave2);
	cJSON_AddNumberToObject(root, "wave3", hub->wave3);
	cJSON_AddNumberToObject(root, "wave4", hub->wave4);
	cJSON_AddNumberToObject(root, "speed", hub->speed);
	cJSON_AddNumberToObject(root, "speed_mult", hub->speed_mult);

	cJSON* banks = cJSON_CreateArray();
	for (int i = 0; i < VBL_NUM_BANKS; i++)
	{
		cJSON* jsb = _serialize_bank(&hub->banks[i]);
		cJSON_AddItemToArray(banks, jsb);
		//cJSON_Delete(jsb);
	}
	cJSON* assignments = cJSON_CreateArray();
	for (int i = 0; i < VBL_NUM_BANKS; i++)
	{
		cJSON* jsb = cJSON_CreateNumber(hub->assignments[i]);
		cJSON_AddItemToArray(assignments, jsb);
	}

	cJSON_AddItemToObject(root, "assignments", assignments);

	cJSON_AddItemToObject(root, "banks", banks);

	const char* result = cJSON_Print(root);
	cJSON_Delete(root);

	r_write_text_to_file(path, result);
}
