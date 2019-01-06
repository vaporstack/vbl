
//  v_objseq.c
//  v_objseq
//
//  Created by Andrew Macfarlane on 02/22/18.
//  Copyright © 2018 vaporstack. All rights reserved.

#include "v_objseq.h"

#include <r4/r4.h>

#include <cmap/map.h>
#include <libkld/list.h>
#include <string.h>

#ifdef R4_ENABLE_ASSIMP

VObjSeq* v_objseq_create(const char* path, int defer_load)
{

	if (!r_file_exists(path))
	{
		printf("Path doesn't exist, can't create this shiz\n");
	}

	if (!r_directory_contains(path, "*.obj"))
	{
		printf("no obj files in folder!\n");
		return NULL;
	}

	unsigned long count;
	r_directory_count(path, "*.obj", &count);

	char** files = calloc(PATH_MAX * count, sizeof(char));

	struct map_t* map = new_map();
	r_directory_glob(path, "*.obj", files, &count);
	const char* trunk    = NULL;
	const char* seqtrunk = NULL;

	for (unsigned long i = 0; i < count; i++)
	{
		const char* name = r_filename_from_path(files[i]);
		const char* ext  = r_get_filename_extension(name);
		ext		 = r_get_filename_extension(files[i]);

		trunk   = r_get_filename_trunk(name);
		ext     = r_get_filename_trunk(files[i]);
		int num = r_get_filename_trailing_number(name);
		if (num == -1)
			continue;

		seqtrunk = r_get_filename_without_trailing_number(trunk);
		//printf("seqtrunk  [%s]\n", seqtrunk);
		void*       data = map_get(map, seqtrunk);
		kld_list_t* list = NULL;

		if (data)
		{
			list = (kld_list_t*)data;
			if (!list)
			{
				list = new_list();
			}
		}
		else
		{

			list = new_list();
		}
		char* dat = calloc(PATH_MAX, sizeof(char));
		dat       = files[i];
		list_append(list, dat);
		//printf("%s - %d\n", seqtrunk, list->size);

		map_set(map, seqtrunk, list);
	}

	unsigned long map_sz = map_size(map);
	printf("map size: %lu\n", map_sz);
	VObjSeq* seq = calloc(1, sizeof(VObjSeq));

	if (seqtrunk)
	{
		void*       data = map_get(map, seqtrunk);
		kld_list_t* list = (kld_list_t*)data;

		while (!list_is_empty(list))
		{
			kld_list_node_t* n = list_pop(list);

			char* fpath = (char*)n->data;
			//printf("%s\n", fpath);
			DAssimpRec* rec = r_assimp_create(fpath, defer_load);
			if (!seq->data)
			{
				seq->data = calloc(1, sizeof(DAssimpRec));
			}
			else
			{
				seq->data = realloc(seq->data, seq->num + 1);
			}
			seq->data[seq->num] = rec;
			seq->num++;
		}
		list_clear(list);
		free(list);
	}

	destroy_map(&map);

	return seq;
}

void v_objseq_load(VObjSeq* seq)
{
	for (int i = 0; i < seq->num; i++)
	{
		if (!seq->data[i]->loaded)
		{
			r_assimp_load(seq->data[i]);
		}
	}
}

void v_objseq_destroy(VObjSeq* seq)
{
	for (int i = 0; i < seq->num; i++)
	{
		r_assimp_destroy(seq->data[i]);
	}
}

#endif
