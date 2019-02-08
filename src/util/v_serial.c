//  v_serial.c
//
//  vbl
//
//  Created by Andrew Macfarlane on 4/22/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "v_serial.h"

#include <r4/r4.h>
#include <vbl/src/core/vbl_log.h>
#include <vbl/vbl.h>

#ifndef RPLATFORM_IOS
#include <cjson/cJSON.h>

extern DebugSettings debug_settings;

/*
cJSON* 			v_serialize_track_json(VSeqTrack* track)
{
	//cJSON *jtrack = cJSON_CreateObject();
	cJSON *jmarks = cJSON_CreateArray();

	for ( int i=0;i< track->mark_num; ++i )
	{
		VSeqMark* m = &track->marks[i];
		cJSON* jmark = cJSON_CreateObject();
		cJSON_AddNumberToObject(jmark, "pos",m->pos);
		cJSON_AddNumberToObject(jmark, "duration",m->duration);
		cJSON_AddNumberToObject(jmark, "id",m->id);
		cJSON_AddItemToArray(jmarks, jmark);

	}

	return jmarks;
}
*/

const char* v_serialize_sequence_json_v001(void* data)
{

	VSeq*  seq  = data;
	cJSON* root = cJSON_CreateObject();

	cJSON* info = cJSON_CreateObject();

	char buf[128];
	sprintf(buf, "%d.%d.%d", VBL_V_MAJOR, VBL_V_MINOR, VBL_V_POINT);
	cJSON_AddStringToObject(info, "version", buf);

	cJSON_AddStringToObject(info, "ref", seq->ref);

	cJSON_AddItemToObject(root, "info", info);

	cJSON_AddNumberToObject(root, "track_cur", seq->track_cur);
	cJSON_AddNumberToObject(root, "track_num", seq->track_num);
	cJSON_AddNumberToObject(root, "track_max", seq->track_max);
	//	theoretically we don't need to store this since it's a simple
	// calc
	cJSON_AddNumberToObject(root, "pos", seq->pos);

	cJSON_AddNumberToObject(root, "time_start", seq->time_start);
	cJSON_AddNumberToObject(root, "time_cur", seq->time_cur);
	cJSON_AddNumberToObject(root, "time_duration", seq->time_duration);

	cJSON* jtracks = cJSON_CreateArray();
	if (debug_settings.serial)
		printf("Serializing %d tracks\n", seq->track_num);
	for (int i = 0; i < seq->track_num; ++i)
	{
		VSeqTrack* t = seq->tracks[i];
		if (!t)
		{
			// printf("NULL TRACK\n");
			continue;
		}
		cJSON* jmarks = cJSON_CreateArray();
		for (int j = 0; j < t->mark_num; ++j)
		{
			VSeqMark* m = &t->marks[j];
			if (debug_settings.serial)
				printf("%d marks to save.\n", t->mark_num);

			cJSON* jmark = cJSON_CreateObject();
			cJSON_AddNumberToObject(jmark, "pos", m->pos);
			cJSON_AddNumberToObject(jmark, "dur", m->dur);
			cJSON_AddNumberToObject(jmark, "id", m->id);
			cJSON_AddItemToArray(jmarks, jmark);
		}
		cJSON_AddItemToObject(jtracks, "marks", jmarks);

		// cJSON* jtrack= v_serialize_track_json(t);
		// cJSON_AddItemToArray(jtracks, jtrack);
	}
	cJSON_AddItemToObject(root, "tracks", jtracks);
	const char* result = cJSON_Print(root);
	if (debug_settings.serial)
		printf("%s\n", result);
	return result;
}

static cJSON* serialize_track_json_v002(VSeqTrack* track)
{
	cJSON* jv = cJSON_CreateObject();

	if (!track)
	{
		// printf("NULL TRACK\n");
		return NULL;
	}

	cJSON* jmarks = cJSON_CreateArray();
	for (int j = 0; j < track->mark_num; ++j)
	{
		VSeqMark* m = &track->marks[j];
		//if (debug_settings.serial)
		//	printf("%d marks to save.\n", t->mark_num);

		cJSON* jmark = cJSON_CreateObject();
		cJSON_AddNumberToObject(jmark, "pos", m->pos);
		cJSON_AddNumberToObject(jmark, "dur", m->dur);
		cJSON_AddNumberToObject(jmark, "id", m->id);
		cJSON_AddItemToArray(jmarks, jmark);
	}
	cJSON_AddBoolToObject(jv, "locked", track->locked);
	cJSON_AddItemToObject(jv, "marks", jmarks);

	//cJSON_AddItemToObject(jtracks, "marks", jmarks);

	// cJSON* jtrack= v_serialize_track_json(t);
	// cJSON_AddItemToArray(jtracks, jtrack);

	return jv;
}

static const char* v_serialize_sequence_json_v002(VSeq* seq)
{

	cJSON* root = cJSON_CreateObject();

	cJSON* info = cJSON_CreateObject();

	cJSON_AddStringToObject(info, "version", VBL_VERSION);

	cJSON_AddStringToObject(info, "ref", seq->ref);

	cJSON_AddItemToObject(root, "info", info);

	cJSON_AddNumberToObject(root, "track_cur", seq->track_cur);
	cJSON_AddNumberToObject(root, "track_num", seq->track_num);
	cJSON_AddNumberToObject(root, "track_max", seq->track_max);
	//	theoretically we don't need to store this since it's a simple
	// calc
	cJSON_AddNumberToObject(root, "pos", seq->pos);

	cJSON_AddNumberToObject(root, "time_start", seq->time_start);
	cJSON_AddNumberToObject(root, "time_cur", seq->time_cur);
	cJSON_AddNumberToObject(root, "time_duration", seq->time_duration);

	if (debug_settings.serial)
		printf("Serializing %d tracks\n", seq->track_num);

	cJSON* jtracks = cJSON_CreateArray();

	for (int i = 0; i < seq->track_num; i++)
	{
		cJSON_AddItemToArray(jtracks, serialize_track_json_v002(seq->tracks[i]));
	}

	/*for (int i = 0; i < seq->track_num; ++i)
	{

		VSeqTrack* t = seq->tracks[i];
		if (!t)
		{
			// printf("NULL TRACK\n");
			continue;
		}
		cJSON* jmarks = cJSON_CreateArray();
		for (int j = 0; j < t->mark_num; ++j)
		{
			VSeqMark* m = &t->marks[j];
			if (debug_settings.serial)
				printf("%d marks to save.\n", t->mark_num);

			cJSON* jmark = cJSON_CreateObject();
			cJSON_AddNumberToObject(jmark, "pos", m->pos);
			cJSON_AddNumberToObject(jmark, "dur", m->dur);
			cJSON_AddNumberToObject(jmark, "id", m->id);
			cJSON_AddItemToArray(jmarks, jmark);
		}
		cJSON_AddItemToObject(jtracks, "marks", jmarks);

		// cJSON* jtrack= v_serialize_track_json(t);
		// cJSON_AddItemToArray(jtracks, jtrack);
	}
	 */

	cJSON_AddItemToObject(root, "tracks", jtracks);
	const char* result = cJSON_Print(root);
	if (debug_settings.serial)
		printf("%s\n", result);
	return result;
}

const char* v_serialize_sequence_json(void* data)
{
	VSeq*       seq     = data;
	const char* version = vbl_get_version_string();
	if (0 == strcmp(version, "0.0.1"))
	{
		return v_serialize_sequence_json_v001(seq);
	}
	else if (0 == strcmp(version, "0.0.2"))
	{
		return v_serialize_sequence_json_v002(seq);
	}
	printf("No code written for serializing this version!\n");
	return NULL;
}

static void fixup_old_style_version_string(char* buf)
{
	int index = 0;
	do
	{
		char c = buf[index];

		if (c == '\0')
			return;

		if (c == '_')
			buf[index] = '.';
		index++;
	} while (true);
}

static void* unserialize_sequence_json_v001(cJSON* root, cJSON* info, const char* version)
{

	const char* ref = cJSON_GetObjectItem(info, "ref")->valuestring;

	VSeq* seq       = v_seq_create();
	seq->ref	= ref;
	seq->version    = version;
	seq->track_cur  = cJSON_GetObjectItem(root, "track_cur")->valueint;
	seq->track_num  = cJSON_GetObjectItem(root, "track_num")->valueint;
	seq->track_max  = cJSON_GetObjectItem(root, "track_max")->valueint;
	seq->pos	= cJSON_GetObjectItem(root, "pos")->valuedouble;
	seq->time_cur   = cJSON_GetObjectItem(root, "time_cur")->valuedouble;
	seq->time_start = cJSON_GetObjectItem(root, "time_start")->valuedouble;
	seq->time_duration =
	    cJSON_GetObjectItem(root, "time_duration")->valuedouble;
	cJSON* jtracks = cJSON_GetObjectItem(root, "tracks");
	int    tnum    = cJSON_GetArraySize(jtracks);
	for (int i = 0; i < tnum; ++i)
	{
		cJSON* jtrack = cJSON_GetArrayItem(jtracks, i);
		int    mnum   = cJSON_GetArraySize(jtrack);
		if (debug_settings.serial)
			printf("%d marks\n", mnum);
		VSeqTrack* track = v_seq_track_create();
		seq->tracks[i]   = track;
		track->marks     = calloc(mnum, sizeof(VSeqMark));
		track->mark_num  = mnum;
		for (int j = 0; j < mnum; ++j)
		{
			cJSON*   jmark = cJSON_GetArrayItem(jtrack, j);
			VSeqMark m;
			m.id		= cJSON_GetObjectItem(jmark, "id")->valueint;
			m.pos		= cJSON_GetObjectItem(jmark, "pos")->valuedouble;
			m.dur		= cJSON_GetObjectItem(jmark, "dur")->valuedouble;
			track->marks[j] = m;
		}

		seq->cur_track = seq->tracks[seq->track_cur];
	}

	return seq;
}

static VSeq* unserialize_sequence_json_v002(cJSON* root, cJSON* info, const char* version)
{

	const char* ref = cJSON_GetObjectItem(info, "ref")->valuestring;

	VSeq* seq       = v_seq_create();
	seq->ref	= ref;
	seq->version    = version;
	seq->track_cur  = cJSON_GetObjectItem(root, "track_cur")->valueint;
	seq->track_num  = cJSON_GetObjectItem(root, "track_num")->valueint;
	seq->track_max  = cJSON_GetObjectItem(root, "track_max")->valueint;
	seq->pos	= cJSON_GetObjectItem(root, "pos")->valuedouble;
	seq->time_cur   = cJSON_GetObjectItem(root, "time_cur")->valuedouble;
	seq->time_start = cJSON_GetObjectItem(root, "time_start")->valuedouble;
	seq->time_duration =
	    cJSON_GetObjectItem(root, "time_duration")->valuedouble;
	cJSON* jtracks = cJSON_GetObjectItem(root, "tracks");
	//int    tnum    = cJSON_GetArraySize(jtracks);
	for (int i = 0; i < seq->track_num; ++i)
	{
		cJSON* jtrack = cJSON_GetArrayItem(jtracks, i);
		cJSON* marks  = cJSON_GetObjectItem(jtrack, "marks");
		cJSON* locked = cJSON_GetObjectItem(jtrack, "locked");
		int    mnum   = cJSON_GetArraySize(marks);

		VSeqTrack* track = v_seq_track_create();
		seq->tracks[i]   = track;
		track->marks     = calloc(mnum, sizeof(VSeqMark));
		track->mark_num  = mnum;

		if (mnum > 100000)
		{
			printf("wtf!\n");
		}

		if (locked)
			track->locked = locked->valueint;

		for (int j = 0; j < mnum; ++j)
		{
			cJSON*   jmark = cJSON_GetArrayItem(marks, j);
			VSeqMark m;
			m.id		= cJSON_GetObjectItem(jmark, "id")->valueint;
			m.pos		= cJSON_GetObjectItem(jmark, "pos")->valuedouble;
			m.dur		= cJSON_GetObjectItem(jmark, "dur")->valuedouble;
			track->marks[j] = m;
		}

		/*int    mnum   = cJSON_GetArraySize(jtrack);
		if (debug_settings.serial)
			printf("%d marks\n", mnum);
		VSeqTrack* track = v_seq_track_create();
		seq->tracks[i]   = track;
		track->marks     = malloc(sizeof(VSeqMark) * mnum);
		track->mark_num  = mnum;
		for (int j = 0; j < mnum; ++j)
		{
			cJSON*   jmark = cJSON_GetArrayItem(jtrack, j);
			VSeqMark m;
			m.id		= cJSON_GetObjectItem(jmark, "id")->valueint;
			m.pos		= cJSON_GetObjectItem(jmark, "pos")->valuedouble;
			m.dur		= cJSON_GetObjectItem(jmark, "dur")->valuedouble;
			track->marks[j] = m;
		}
		*/
	}
	seq->cur_track = seq->tracks[seq->track_cur];

	return seq;
}

void* v_unserialize_sequence_json(const char* path)
{
	const char* data = r_read_file_as_text(path);
	if (debug_settings.serial)
		printf("UNSERIALIZING\n%s\n", data);
	cJSON* root = cJSON_Parse(data);

	cJSON* info = cJSON_GetObjectItem(root, "info");
	if (!info)
	{
		l_warning("Error reading info from work file.\n");
		return NULL;
	}
	cJSON* obj     = cJSON_GetObjectItem(info, "version");
	char*  version = NULL;
	if (obj)
	{
		version = obj->valuestring;
		fixup_old_style_version_string(version);
	}
	if (!version)
	{
		vbl_log("Unable to discern version: %s", path);
		return NULL;
	}
	if (0 == strcmp("0.0.1", version))
	{
		return unserialize_sequence_json_v001(root, info, version);
	}
	else if (0 == strcmp("0.0.2", version))
	{
		return unserialize_sequence_json_v002(root, info, version);
	}

	printf("Unable to unserialize, no code path for this version[%s]\n", version);

	return NULL;
}

//	TODO unify the various stupid file reading functions

void* vbl_read_file_as_bin(const char* path, long* len)
{
	FILE* fileptr;
	// char *buffer;
	// long filelen;
	long l;

	fileptr = fopen(path, "rb"); // Open the file in binary mode
	if (!fileptr)
	{
		return NULL;
	}
	fseek(fileptr, 0, SEEK_END); // Jump to the end of the file
	l = ftell(fileptr);	  // Get the current byte offset in the file
	rewind(fileptr);	     // Jump back to the beginning of the file

	printf("%lu\n", *len);
	long  sz  = (l + 1) * sizeof(char);
	void* buf = malloc(sz); // Enough memory for file + \0
	memset(buf, 0, sz);
	fread(buf, l, 1, fileptr); // Read in the entire file
	fclose(fileptr);	   // Close the file
	*len = l;
	return 0;
}

const char* vbl_read_file_as_text(const char* path)
{
	FILE* f;

	f = fopen(path, "r");
	if (!f)
	{
		char buf[256];
		sprintf(buf, "Failed to open file: %s\n", path);
		printf("%s\n", buf);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); // same as rewind(f);

	char* data = malloc(fsize + 1);
	fread(data, fsize, 1, f);
	fclose(f);

	data[fsize] = 0;

	return data;
}

char* vbl_read_file_as_text_nc(const char* path)
{
	FILE* f;

	f = fopen(path, "r");
	if (!f)
	{
		char buf[256];
		sprintf(buf, "Failed to open file: %s\n", path);
		printf("%s\n", buf);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); // same as rewind(f);

	char* data = malloc(fsize + 1);
	fread(data, fsize, 1, f);
	fclose(f);

	data[fsize] = 0;

	return data;
}

int vbl_write_text_to_file(const char* path, const char* data)
{
	FILE* f;
	f = fopen(path, "w");
	if (!data)
	{
		printf("Data was null!\n");
		return 1;
	}
	if (!f)
	{
		char buf[256];
		sprintf(buf, "Failed to open file: %s\n", path);
		printf("%s\n", buf);
		// printf("Failed to open file: %s\n", path);
		return 1;
	}

	fprintf(f, "%s", data);
	fclose(f);

	return 0;
}

#endif
