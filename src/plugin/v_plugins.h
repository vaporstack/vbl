//
//  v_plugin.h
//  vbl
//
//  Created by Andrew Macfarlane on 11/10/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_plugin_h
#define v_plugin_h

#include <r4/src/r4_config.h>

#ifdef R4_ENABLE_V_PLUGIN

#include <r4/src/io/r_portmidi.h>

typedef void* (*plugin_create)(void);
typedef void (*plugin_destroy)(void*);
typedef int (*plugin_update)(void*);
typedef int (*plugin_send)(void*, void*);

enum vbl_plugin_type
{
	VBLIT_PLUGIN_TYPE_INPUT,
	VBLIT_PLUGIN_TYPE_MIDI,
	VBLIT_PLUGIN_TYPE_OSC,
	VBLIT_PLUGIN_TYPE_ROUTINE
};

typedef struct VBlitPlugin
{
	int type;

	plugin_create  create;
	plugin_destroy destroy;
	plugin_update  update;
	plugin_update  init;
	plugin_update  deinit;
	plugin_send    send;

	int  enabled;
	int  active;
	bool needs_update_every_frame;

	midi_func callback;
	void*     data;

	char* version;
	char* identifier;
	char* filename;
	char* name;
	char* path;
} VBlitPlugin;

void v_plugins_enumerate(void);
void v_plugins_load(const char* path);
void v_plugins_unload(void);

void v_plugins_init(void);
void v_plugins_deinit(void);

int v_plugins_query(const char* identifier);

void	 v_plugins_update(void);
VBlitPlugin* v_plugins_instance(const char* identifier);

void v_plugin_update(VBlitPlugin*);

#endif // RPLATFORM_IOS

#endif /* v_plugin_h */
