
//  v_channel.c
//  v_channel
//
//  Created by Andrew Macfarlane on 11/29/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#ifndef v_channel_h_
#define v_channel_h_

#include <gl-matrix/gl-matrix.h>

/*
 *	class for linking inputs (midi, osc, etc)
 *	to outputs (variables, vectors, etc)
 */

enum V_CHANNEL_CONTROL_TYPES
{
	V_CHANNEL_CONTROL_BOOL,
	V_CHANNEL_CONTROL_INT,
	V_CHANNEL_CONTROL_LONG,
	V_CHANNEL_CONTROL_FLOAT,
	V_CHANNEL_CONTROL_DOUBLE,
	V_CHANNEL_CONTROL_STRING,
	V_CHANNEL_CONTROL_TRIGGER
};

typedef struct VChannelInput
{
	int   num;
	int   type;
	void* data;
	char* name;
} VChannelInput;

typedef struct VChannelInputHnd
{
	VChannelInput* src;
} VChannelInputHnd;

typedef struct VChannelOutput
{

	int   num;
	int   type;
	void* data;
	char* name;
} VChannelOutput;

typedef struct VChannelOutputHnd
{
	VChannelInput* src;
} VChannelOutputHnd;

typedef struct VChannel
{
	VChannelInput*  input;
	VChannelOutput* output;
	const char*     name;
	int		type;
} VChannel;

typedef struct VNetwork
{

} VNetwork;

extern struct vector_t connections;
extern struct vector_t v_inputs;
extern struct vector_t v_outputs;

extern struct map_t* outputs;
extern struct map_t* inputs;

//extern int  v_channel_initialized;
extern bool automatic_link;

VChannel* v_channel_create(void);

void v_channel_init(void);
void v_channel_deinit(void);
void v_channel_update(void);

int v_channel_advertise_input_int(const char* name, int* v, int num);
int v_channel_advertise_input_long(const char* name, long* v, int num);
int v_channel_advertise_input_bool(const char* name, int* v, int num);
int v_channel_advertise_input_float(const char* name, float* v, int num);
int v_channel_advertise_input_double(const char* name, double* v, int num);
int v_channel_advertise_input_string(const char* name, char* str, int num);
int v_channel_advertise_input_trigger(const char* name, void*, int num);

int v_channel_advertise_output_int(const char* name, int* v, int num);
int v_channel_advertise_output_long(const char* name, long* v, int num);
int v_channel_advertise_output_bool(const char* name, int* v, int num);
int v_channel_advertise_output_float(const char* name, float* v, int num);
int v_channel_advertise_output_double(const char* name, double* v, int num);
int v_channel_advertise_output_string(const char* name, char* str, int num);
int v_channel_advertise_output_trigger(const char* name, void*, int num);
// int v_channel_advertise_input_vec3(const char* name, vec3_t* vec, int num);

#endif
