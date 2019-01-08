
//  v_swarm.c
//  v_swarm
//
//  Created by Andrew Macfarlane on 11/20/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "v_swarm.h"

#include <drw/drw.h>
#include <grdn/grdn.h>
#include <r4/r4.h>
#include <stdio.h>

extern AppSettings app_settings;

static swarm_draw_fun draw = NULL;

VSwarm* v_swarm_create(int num)
{

	VSwarm* swarm	= calloc(1, sizeof(VSwarm));
	swarm->which	 = 0;
	GParticleSystem* sys = g_particle_system_create(num);
	// g_particle_system_set_wind(sys, .01,.2,0);
	// g_particle_system_set_gravity(sys, 0,-.2,0);
	swarm->sys = sys;
	if (!sys)
	{
		printf("Unable to create particle system!\n");
		free(swarm);
		return NULL;
	}

	//sys->death	     = 5;
	GParticleEmitter* emit = g_particle_emitter_create(sys);
	g_particle_system_set_emitter(sys, emit);
	// swarm->sys->emitter = emit;
	swarm->num = VSWARM_MAX_BEHAVIORS;
	for (int i = 0; i < VSWARM_MAX_BEHAVIORS; i++)
	{
		swarm->behave[i] = NULL;
	}

	for (int i = 0; i < SWARM_MAX; i++)
	{
		// swarm->trails[i]
		// printf("rline3 : %d\n", swarm->trails[i].num);
	}
	swarm->next    = NULL;
	swarm->current = NULL;

	// swarm->trails = r_object_create();
	return swarm;
}

void v_swarm_destroy(VSwarm* swarm)
{
	for (int i = 0; i < swarm->sys->max; i++)
	{
		// r_line3_destroy(<#RLine3 *line#>)
	}
	g_particle_system_destroy(swarm->sys);
	free(swarm);
}

void v_swarm_set_draw_func(swarm_draw_fun fun)
{
	draw = fun;
}

void v_swarm_update(VSwarm* swarm)
{

	GParticleSystem* sys = swarm->sys;

	g_particle_system_update(swarm->sys);

	for (int i = 0; i < sys->max-1; i++)
	{

		Particle* p = sys->data[i];
		if (!p)
		{
			RLine3* myline = &swarm->trails[i];
			r_line3_clear(myline);

			continue;
		}
		int behavior = p->extra[0];

		VBehave* behave = swarm->behave[behavior];

		RLine3* line = &swarm->trails[i];
		r_line3_add_point3f(line, p->position[0], p->position[1],
				    p->position[2]);

		if (line->num > TRAIL_LENGTH)
		{

			// int offset = line->num - TRAIL_LENGTH;
			// printf("Need to truncate line by %d points.\n",
			// offset);
			size_t sz = sizeof(RPoint3) * TRAIL_LENGTH;
			memmove(&line->data[0], &line->data[1], sz);

			line->num = TRAIL_LENGTH;
		}

		if (behave)
		{
			//	somethin
			// printf("doin it\n");
			if (behave->move)
				behave->move(sys, p);

			if (behave->test)
			{
				if (behave->test(sys, p, behave->distance))
				{

					if (behave->end)
					{
						//printf("triggered end behavior!\n");
						behave->end(sys, p);
					}
					behavior++;
					if (behavior > swarm->num)
						behavior = 0;

					p->extra[0] = behavior;
					/*
				 p->data++;
				if ( p->data > swarm->num)
					p->data = 0;
				*/
					// printf("Particle %d is now doing
					// %d\n", i, behavior);
				}
			}
		}
	}
}

#define TMP_SIZE 32
void _particle_draw_debug(GParticleSystem* sys)
{
	double w = 1; //app_settings.framebuffer_width;
	double h = 1; //app_settings.framebuffer_height;

	drw_push();
	drw_translate2f(w * -.5, h * -.5);
	drw_translate2f(TMP_SIZE, TMP_SIZE);
	// double w = app_settings.
	for (int i = 0; i < sys->max; i++)
	{
		drw_translate2f(TMP_SIZE, 0);
		if (sys->data[i])
			drw_square(TMP_SIZE);
	}
	drw_pop();
	drw_color(1, 1, 1, .5);
	g_particle_system_draw_debug(sys);
}

void v_swarm_draw(VSwarm* swarm)
{
	if (draw)
	{
		draw(swarm);
		return;
	}

	for (int i = 0; i < swarm->sys->max; i++)
	{

		Particle* p = swarm->sys->data[i];
		if (!p)
			continue;

		if (p->draw)
		{
			p->draw(p);
		}
		else
		{
			drw_push();
			drw_translate_vec3(p->position);
			// drw_scale_u(p->size);
			// drw_rotate_vec3(p->rotation);
			if (draw)
			{
			}
			else
			{
				drw_square(8);
			}

			drw_pop();
			for (int j = 0; j < swarm->sys->max; j++)
			{
				Particle* p2 = swarm->sys->data[j];
				if (!p2 || !p)
					continue;
				//vec3_t po1 = p->position;
				//vec3_t po2 = p2->position;

				drw_line3_r(&swarm->trails[j]);
				/*
				 int pos =
				    r_rand_long(NULL) % swarm->trails[j].num;
				RPoint3 point = swarm->trails[i].data[pos];
				drw_line_3f(
				    point.x, point.y, point.z, point.x,
				    point.y +
					((-.5 + r_rand_double(NULL)) * 128),
				    point.z);
				*/

				// drw_line_3f(po1[0],po1[1],po1[2],po2[0],po2[1],
				// po2[2]);
			}
		}
	}
	if (app_settings.debug)
	{
		_particle_draw_debug(swarm->sys);
	}
}
