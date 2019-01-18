//
//  v_plugin.c
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "v_plugins.h"

#ifdef R4_ENABLE_V_PLUGIN

#include "include/tinydir.h"
#include <dlfcn.h>
#include <mkdirp/mkdirp.h>
#include <wildcardcmp/wildcardcmp.h>

#include <vector/vector.h>

#include <r4/r4.h>
#include <r4/src/io/r_portmidi.h>

static struct vector_t plugins;
extern DebugSettings   debug_settings;

VBlitPlugin* v_plugin_load(const char* path, const char* name);
// VBlitPlugin** plugins = NULL;

//	todo; load and init are sort of confusing here. I'd expect init to be first

//	path is optional here. default path otherwise.
void v_plugins_load(const char* path)
{

	int res = vector_init(&plugins, 4, sizeof(VBlitPlugin));
	// printf("vector init res: %d\n", res);

	char buf[256];
	if (path)
	{
		sprintf(buf, "%s", path);
	}
	else
	{
		char* directory = r_get_path_data();
		sprintf(buf, "%s/plugins", directory);
		free(directory);
	}

	//if (debug_settings.plugin)
		printf("Scanning for plugins in directory: %s\n", buf);
	int successes = 0;

	mkdirp(buf, (mode_t)0755);

	tinydir_dir dir;
	tinydir_open(&dir, buf);
	// char** plugins = NULL;
	int num_plugins = 0;
	debug_settings.plugin = true;
	
	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (strcmp(file.name, ".") != 0 &&
		    strcmp(file.name, "..") != 0)
		{
			if (wildcardcmp("*.plugin", file.name))
			{

				if (!r_file_exists(file.path))
				{
					printf(".so not found\n");
					continue;
				}
				VBlitPlugin* res = v_plugin_load(file.path, file.name);
				if (res)
				{
					if (debug_settings.plugin)
						printf(
						    "Plugin load succeeded!\n");
					// VBlitPlugin* plug = calloc(1,
					// sizeof(VBlitPlugin));
					vector_push_back(&plugins, res);
					successes++;
				}
				else
				{
					if (debug_settings.plugin)
						printf("Plugin load failed!\n");
				}
			}
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	//if (debug_settings.plugin)
	printf("Succesfully loaded %d plugins.\n", successes);
	for (int i = 0; i < plugins.length; i++)
	{
		// printf("%s\n", p)
	}
}

void v_plugin_unload(VBlitPlugin* plug);

void v_plugins_unload(void)
{
	for (int i = 0; i < plugins.length; i++)
	{
		VBlitPlugin* plug = calloc(1, sizeof(VBlitPlugin));
		vector_get(&plugins, i, plug);
		v_plugin_unload(plug);
		// free(plug);
	}
}

void v_plugins_init(void)
{
	for (int i = 0; i < plugins.length; i++)
	{
		VBlitPlugin* plug = calloc(1, sizeof(VBlitPlugin));
		vector_get(&plugins, i, plug);
		if (plug->init)
		{
			plug->init(plug);
		}
		free(plug);
	}
}

void v_plugins_deinit(void)
{
	for (int i = 0; i < plugins.length; i++)
	{
		VBlitPlugin* plug = calloc(1, sizeof(VBlitPlugin));
		vector_get(&plugins, i, plug);
		if (plug->deinit)
		{
			plug->deinit(plug);
		}
		free(plug);
	}
}

VBlitPlugin* v_plugin_load(const char* path, const char* name)
{
	if (debug_settings.plugin)
		printf("Attempting to load plugin:\n %s\n", path);

	char buf[PATH_MAX];

	sprintf(buf, "%s%slib%s.%s", path, R4_PATH_SEP, name, R4_SHAREDLIB_EXT);

	if (debug_settings.plugin)
		printf("Looking for dll: %s\n", buf);

	if ( ! r_file_exists(buf))
	{
		printf("DLL doesn't exist!\n");
		return NULL;
		
	}
	
	void* handle;
	void* (*vblit_plugin_create)(void);
	// double (*cosine)(double);
	// void* (*vblit_plugin_destroy)(void*);
	// char* error;

	handle = dlopen(buf, RTLD_LAZY);
	if (!handle)
	{
		l_warning("%s\n", dlerror());
		return NULL;
	}

	dlerror(); // Clear any existing error

	// Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
	//    would seem more natural, but the C99 standard leaves
	//   casting from "void *" to a function pointer undefined.
	//   The assignment used below is the POSIX.1-2003 (Technical
	//    Corrigendum 1) workaround; see the Rationale for the
	//    POSIX specification of dlsym().

	*(void**)(&vblit_plugin_create) = dlsym(handle, "vblit_plugin_create");
	// fprintf(stderr, "%s\n", dlerror());

	void* foo = vblit_plugin_create();

	VBlitPlugin* plug = (VBlitPlugin*)foo;
	if (!plug)
	{
		return NULL;
	};

	if (debug_settings.plugin)
		printf("[PLUGIN] : %s %s\n", plug->version, plug->identifier);

	plug->path     = strdup(path);
	plug->filename = strdup(name);

#ifdef R4_ENABLE_PORTMIDI
	if (plug->type == VBLIT_PLUGIN_TYPE_MIDI)
	{
		l_info("Setting PORT MIDI CALLBACK\n");
		plug->callback = r_portmidi_receive;
	}
#endif

#ifdef R4_ENABLE_RTMIDI
	if (plug->type == VBLIT_PLUGIN_TYPE_MIDI)
	{

		plug->callback = r_rtmidi_receive;
		//l_warning("fixme: RTMIDI not yet implemented (no effect)");
		//printf("fixme: (RTMIDI) NOT setting a  midi callback because I haven't written it yet.\n");
	}
#endif

	plug->active  = true;
	plug->enabled = true;

	//	todo: remove this hack
	if (0 == strcmp(plug->identifier,
			"space.ruminant.v_spurious_midi_emitter"))
	{
		printf("Disabling the spurious midi emitter!\n");
		plug->active  = false;
		plug->enabled = false;
	}

	return plug;
}

void v_plugin_unload(VBlitPlugin* plug)
{

	printf("Unloading all plugins.\n");

	printf("Attempting to unload plugin:\n %s\n", plug->path);
	char buf[PATH_MAX];

	sprintf(buf, "%s%slib%s.%s", plug->path, R4_PATH_SEP, plug->filename,
		R4_SHAREDLIB_EXT);
	printf("Looking for dll: %s\n", buf);

	void* handle;

	void* (*vblit_plugin_destroy)(void*);
	char* error;

	handle = dlopen(buf, RTLD_LAZY);
	if (!handle)
	{
		l_warning("%s\n", dlerror());
		return;
	}

	dlerror(); // Clear any existing error

	// Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
	//    would seem more natural, but the C99 standard leaves
	//   casting from "void *" to a function pointer undefined.
	//   The assignment used below is the POSIX.1-2003 (Technical
	//    Corrigendum 1) workaround; see the Rationale for the
	//    POSIX specification of dlsym().

	*(void**)(&vblit_plugin_destroy) =
	    dlsym(handle, "vblit_plugin_destroy");

	fprintf(stderr, "%s\n", dlerror());

	if (debug_settings.plugin)
		printf("Destroying plugin: %s\n", plug->filename);
	vblit_plugin_destroy(plug);

	// *(void**)(&cosine) = dlsym(handle, "cos");

	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);
		return;
	}

	dlclose(handle);
}

void v_plugin_update(VBlitPlugin* plug)
{
	if (!plug->update)
	{
		printf("No update method set!\n");
		return;
	}
	plug->update(plug);
}

void v_plugins_update(void)
{
	VBlitPlugin* plug = calloc(1, sizeof(VBlitPlugin));
	for (int i = 0; i < plugins.length; i++)
	{
		vector_get(&plugins, i, plug);

		if (plug->enabled && plug->active)

		{
			if (plug->needs_update_every_frame)
				v_plugin_update(plug);
		}
	}
	free(plug);
}

void v_plugins_enumerate(void)
{

	for (int i = 0; i < plugins.length; i++)
	{
		VBlitPlugin* plug = calloc(1, sizeof(VBlitPlugin));
		vector_get(&plugins, i, plug);
		printf("plugin: %s\n", plug->identifier);
		free(plug);
	}
}
int v_plugins_query(const char* identifier)
{
	return -1;
}

VBlitPlugin* v_plugins_instance(const char* identifier)
{
	return NULL;
}

#endif
