
//  v_channel.c
//  v_channel
//
//  Created by Andrew Macfarlane on 11/29/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_channel.h"

#include <cmap/map.h>
#include <vector/vector.h>

#include <stdint.h>

#include <stdio.h>

#include <stdlib.h>

//	TODO: Make these be static, right now they're polluting the global namespace
struct vector_t connections;
struct vector_t v_inputs;
struct vector_t v_outputs;

struct map_t* outputs = NULL;
struct map_t* inputs  = NULL;

static int initialized    = 0;
bool       automatic_link = true;

void v_channel_check_init(void)
{
	printf("lazy init check: %d\n", initialized);
	if (!initialized)
	{
		printf("oops, v_channel_input was called before init.  Lazy "
		       "init, buuuuut\n");
		v_channel_init();
	}
}

void v_channel_init(void)
{
	if (initialized)
	{
		printf("Already initialized oops!\n");
		return;
	}
	inputs  = new_map();
	outputs = new_map();

	vector_init(&connections, 8, sizeof(VChannel));
	vector_init(&v_inputs, 8, sizeof(VChannelInput));
	vector_init(&v_outputs, 8, sizeof(VChannelOutput));

	initialized = true;
	// vector_init(&inputs, 8, sizeof(VChannelInput));
	// vector_init(&outputs, 8, sizeof(VChannelOutput));
}

void v_channel_deinit(void)
{
	destroy_map(&inputs);
	destroy_map(&outputs);
}

static int make_link(VChannelInput* input, VChannelOutput* output)
{
	printf("Making link!\n");
	VChannel* link = calloc(1, sizeof(VChannel));
	link->input    = input;
	link->output   = output;
	vector_push_back(&connections, link);
	printf("MADE LINK!\n");
	return 0;
}

static int automatic_link_input(VChannelInput* input)
{
	printf("Checking for automatic link (i):\n");
	VChannelOutput* tmp = calloc(1, sizeof(VChannelOutput));
	printf("%zu output candidates to check.\n", v_outputs.length);
	for (int i = 0; i < v_outputs.length; i++)
	{
		vector_get(&v_outputs, i, tmp);
		if (tmp->type == input->type)
		{
			printf("Candidate!!\n");
			return make_link(input, tmp);
		}
	}
	free(tmp);

	return 1;
}

static int automatic_link_output(VChannelOutput* output)
{
	printf("Checking for automatic link (o):\n");

	VChannelInput* tmp = calloc(1, sizeof(VChannelInput));
	bool succeeded = false;
	for (int i = 0; i < v_inputs.length; i++)
	{
		vector_get(&v_inputs, i, tmp);
		if (tmp->type == output->type)
		{
			printf("Candidate!!\n");
			make_link(tmp, output);
			succeeded = true;
		}
	}
	if( !succeeded )
		free(tmp);
	return 1;
}

static int create_output(const char* name, void* data, int num, int type)
{
	VChannelOutput* test = map_get(outputs, name);
	if (test)
	{
		printf("Already have an output of this name\n");
		return 1;
	}
	VChannelOutput* output = calloc(1, sizeof(VChannelOutput));

	output->type = type;
	output->data = data;
	output->num  = num;

	vector_push_back(&v_outputs, output);
	map_set(outputs, name, output);
	printf("Registered output %s %p %d [%zu] outputs.\n", name, data, type, v_outputs.length);
	if (automatic_link)
	{
		return automatic_link_output(output);
	}
	return 0;
}

static int create_input(const char* name, void* data, int num, int type)
{
	VChannelInput* test = map_get(inputs, name);
	if (test)
	{
		printf("Already haaaave an input of this name\n");
		return 1;
	}
	VChannelInput* input = calloc(1, sizeof(VChannelInput));

	input->type = type;
	input->data = data;
	input->num  = num;

	vector_push_back(&v_inputs, input);
	map_set(inputs, name, input);
	printf("Registered input %s %p %d [%zu] inputs.\n", name, data, type, v_inputs.length);
	if (automatic_link)
	{
		int res = automatic_link_input(input);
		printf("Automatic link operation: %d\n", res);
	}
	return 0;
}

void v_channel_update(void)
{
	//	loop through channels and push vals from ins to outs
	for (int i = 0; i < connections.length; i++)
	{
		VChannel* chan = calloc(1, sizeof(VChannel));
		// VChannel* chan = connections[i];
		vector_get(&connections, i, chan);
		VChannelInput*  input  = chan->input;
		VChannelOutput* output = chan->output;
		int		cnum   = output->num;
		// printf("pushing value to %d channels\n", cnum);

		for (int j = 0; j < cnum; j++)
		{
			// output->data = input->data;

			switch (output->type)
			{
			case V_CHANNEL_CONTROL_BOOL:
			case V_CHANNEL_CONTROL_INT:
			{
				int foo = *(int*)input->data;
				printf("ifoo: %d\n", foo);
				break;
			}
			case V_CHANNEL_CONTROL_LONG:
			{
				long  foo = *(long*)input->data;
				long* bar = (long*)output->data;
				//*output->data = foo;
				*bar = foo;
				printf("lfoo: %lu\n", foo);
				break;
			}
			case V_CHANNEL_CONTROL_FLOAT:
			{
				float  foo = *(float*)input->data;
				float* bar = (float*)output->data;
				*bar       = foo;
				printf("ffoo: %f\n", foo);
				break;
			}
			case V_CHANNEL_CONTROL_DOUBLE:
			{
				double foo = *(double*)input->data;
				printf("dfoo: %f\n", foo);
				break;
			}
			default:
				break;
			}
			// output->data[j] = input->data[j];
		}
	}
}

int v_channel_advertise_input_float(const char* name, float* v, int num)
{
	v_channel_check_init();
	return create_input(name, v, num, V_CHANNEL_CONTROL_FLOAT);
}

int v_channel_advertise_input_double(const char* name, double* v, int num)
{
	v_channel_check_init();
	return create_input(name, v, num, V_CHANNEL_CONTROL_DOUBLE);
}

int v_channel_advertise_input_int(const char* name, int* v, int num)
{
	v_channel_check_init();
	return create_input(name, v, num, V_CHANNEL_CONTROL_INT);
}

int v_channel_advertise_input_long(const char* name, long* v, int num)
{
	v_channel_check_init();
	return create_input(name, v, num, V_CHANNEL_CONTROL_LONG);
}

int v_channel_advertise_input_bool(const char* name, int* v, int num)
{
	v_channel_check_init();
	return create_input(name, v, num, V_CHANNEL_CONTROL_BOOL);
}

int v_channel_advertise_input_string(const char* name, char* v, int num)
{
	v_channel_check_init();
	return create_input(name, v, num, V_CHANNEL_CONTROL_STRING);
}

int v_channel_advertise_output_int(const char* name, int* v, int num)
{
	v_channel_check_init();
	return create_output(name, v, num, V_CHANNEL_CONTROL_INT);
}

int v_channel_advertise_output_long(const char* name, long* v, int num)
{
	v_channel_check_init();
	return create_output(name, v, num, V_CHANNEL_CONTROL_LONG);
}

int v_channel_advertise_output_double(const char* name, double* v, int num)
{
	v_channel_check_init();
	return create_output(name, v, num, V_CHANNEL_CONTROL_DOUBLE);
}

int v_channel_advertise_output_bool(const char* name, int* v, int num);

int v_channel_advertise_output_float(const char* name, float* v, int num)
{
	v_channel_check_init();
	return create_output(name, v, num, V_CHANNEL_CONTROL_FLOAT);
}

// int v_channel_advertise_output_double(const char* name, double* v, int num);
int v_channel_advertise_output_string(const char* name, char* str, int num);
int v_channel_advertise_output_trigger(const char* name, void*, int num);
