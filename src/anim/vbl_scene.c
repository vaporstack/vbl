
//
//  vbl_scene.c
//  vblit
//
//  Created by vs on 2/8/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "vbl_scene.h"

//#include "vbl_common.h"

//#include "generators/v_routine.h"
#include <drw/drw.h>

#include <r4/r4.h>

#include <grdn/src/ent/g_entity.h>
#include <grdn/src/scene/g_camera.h>

//int camera_shake_hit;
static GEntity* camera = NULL;

//double camera_shake_amount;
static bool   debug_scene      = false;
static vec3_t scene_dest       = NULL;
static vec3_t scene_rotate     = NULL;
static vec3_t scene_rotate_vel = NULL;

static float vel = 0;

void vbl_scene_velocity_set(double v)
{
	vel = v;
}

double vbl_scene_velocity_get(void)
{
	return vel;
}

static void lerp(double* a, double* b, double amt)
{
	double d = *b - *a;
	*a += (d * amt);
}

#define TMP_R .05
void vbl_scene_camera_apply(void)
{
	if (!scene_rotate)
	{
		printf("Error, scene_rotate was not allocated!\n");
		return;
	}
	g_camera_apply(camera);
	drw_rotate_vec3(scene_rotate);
}

void vbl_scene_camera_unapply(void)
{
	g_camera_deapply(camera);
	drw_rotate(-scene_rotate[0], -scene_rotate[1], -scene_rotate[2]);
}

static void dlerp(double* a, double* b, double amt)
{
	double d = *b - *a;
	*a += (d * amt);
}

void vbl_scene_update(void)
{

	//if (r_time_get_paused()) {
	//	return;
	//}

	GCamera* cdata = camera->data;
	vec3_t   pos   = cdata->position;
	vec3_t   shake = cdata->shake;
	//cdata->shake_mag = camera_shake_amount;
	//Add a comment to this line

	//shake[0] = camera_shake_amount;
	//shake[1] = camera_shake_amount;
	//s/hake[2] = camera_shake_amount;
	// render_s3(block);
	if (vec3_dist(scene_rotate, scene_dest) != 0)
	{
		lerp(&scene_rotate[0], &scene_dest[0], TMP_R);
		lerp(&scene_rotate[1], &scene_dest[1], TMP_R);
		lerp(&scene_rotate[2], &scene_dest[2], TMP_R);
	}

	double foo = scene_rotate_vel[0];
	double bar = scene_rotate[0];

	if (r_time_get_paused())
	{
		return;
	}

	scene_dest[0] += scene_rotate_vel[0];
	scene_dest[1] += scene_rotate_vel[1];
	scene_dest[2] += scene_rotate_vel[2];

	// pos[1] = 128;
	cdata->position = pos;
	camera->data    = cdata;

	g_camera_update(camera);

	//char buf[256];
	//vec3_str(camera->transform.position, buf);
	//printf("%s\n", buf);

	/*
	if ( vel == 0 )
	{
		l_warning("Shouldn't be updating camera with 0 velocity\n");
		vbl_scene_velocity_set(.01);
	}

	GCamera* cdata = camera->data;
	vec3_t   pos   = cdata->position;
	vec3_t   shake = cdata->shake;

	if (vec3_dist(scene_rotate, scene_dest) != 0)
	{
		lerp(&scene_rotate[0], &scene_dest[0], vel);
		lerp(&scene_rotate[1], &scene_dest[1], vel);
		lerp(&scene_rotate[2], &scene_dest[2], vel);
	}

	scene_dest[0] += scene_rotate_vel[0];
	scene_dest[1] += scene_rotate_vel[1];
	scene_dest[2] += scene_rotate_vel[2];

	//GCamera* cdata = camera->data;
	cdata->position[0] = scene_rotate[0];
	cdata->position[1] = scene_rotate[1];
	cdata->position[2] = scene_rotate[2];
	g_camera_update(camera);


	char buf[256];

	vec3_str(camera->transform.position, buf);
	printf("%s\n", buf);
	//if (r_time_get_paused()) {
	//	return;
	//}

	cdata->position = pos;

	/*

	//cdata->shake_mag = camera_shake_amount;

	//shake[0] = camera_shake_amount;
	//shake[1] = camera_shake_amount;
	//s/hake[2] = camera_shake_amount;
	// render_s3(block);


	double foo = scene_rotate_vel[0];
	double bar = scene_rotate[0];

	if (r_time_get_paused())
	{
		return;
	}

	scene_dest[0] += scene_rotate_vel[0];
	scene_dest[1] += scene_rotate_vel[1];
	scene_dest[2] += scene_rotate_vel[2];

	// pos[1] = 128;
	cdata->position = pos;
	//camera->data    = cdata;

	//g_camera_update(camera);

	 */
}

void vbl_scene_init(void)
{

	scene_rotate     = vec3_create(NULL);
	scene_rotate_vel = vec3_create(NULL);
	scene_dest       = vec3_create(NULL);
	camera		 = g_camera_create();
}

void vbl_scene_deinit(void)
{
	free(scene_rotate);
	free(scene_dest);

	g_camera_destroy(camera);
}

void vbl_scene_adjust_screenrotate_x(double v)
{
	scene_rotate_vel[0] = v;
}
void vbl_scene_adjust_screenrotate_y(double v)
{
	scene_rotate_vel[1] = v;
}
void vbl_scene_adjust_screenrotate_z(double v)
{
	scene_rotate_vel[2] = v;
}
void vbl_scene_camera_shake_hit(void)
{
	/*
	if (val == 0)
	{
		val = 5;
	}
	 */

	/*GCamera* cdata       = camera->data;
	cdata->shake_enabled = true;
	cdata->shake_mag += val;
	if (debug_scene)
	{
		printf("hit!\n");
		printf("camera: %f\n", cdata->shake_mag);
	}
	cdata->shake_decay = true;
	camera->data       = cdata;
	 */
}

void vbl_scene_camera_shake_adjust(void)
{
	/*
	GCamera* cdata       = camera->data;
	cdata->shake_enabled = true;
	cdata->shake_mag     = v * 5;
	if (debug_scene)
		printf("camera: %f\n", cdata->shake_mag);
	cdata->shake_decay = false;

	camera->data = cdata;
	 */
}

static void faces2(double r)
{
}

/*
static void cube(double r)
{
	//	the WORST poss impl but avoid shitty glut ness and I'm in a rush
	drw_push();
	drw_push();
	drw_translate_z(r * -.5);
	drw_square(r);
	drw_type_draw("FRONT");
	drw_translate_z(r);
	drw_type_draw("BACK");

	drw_square(r);
	drw_pop();

	drw_rotate_x(90);

	drw_push();
	drw_translate_z(r * -.5);
	drw_square(r);
	drw_type_draw("TOP");
	drw_translate_z(r);
	drw_type_draw("BOTTOM");

	drw_square(r);
	drw_pop();
	drw_pop();
}
*/

void vbl_scene_draw_debug(void)
{
	//drw_type_set_size(D_FONT_SIZE, 144);

	drw_color_push();
	drw_alpha(.5);
	//double h = app_settings.framebuffer_height;
	drw_cube(.5);
	drw_alpha_pop();
	drw_color_pop();
}

void vbl_scene_rotate_reset(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 0;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_front(void)
{
	scene_dest[0] = 0;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_left(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 0;
	scene_dest[1] = 90;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_right(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);
	scene_dest[0] = 0;
	scene_dest[1] = -90;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_top(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 90;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_bottom(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	scene_dest[0] = -90;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_back(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 0;
	scene_dest[1] = 180;
	scene_dest[2] = 0;
}

static int cardinal_direction_45(void)
{
	int which = (r_rand_double(NULL) * 8);
	int res   = which * 45;
	return res;
}

void vbl_scene_rotate_random(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	static double sz = 360;
	scene_dest[0]    = r_rand_double(NULL) * sz;
	scene_dest[1]    = r_rand_double(NULL) * sz;
	scene_dest[2]    = r_rand_double(NULL) * sz;
}

void vbl_scene_rotate_random_constrained(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	static double sz = 360;
	scene_dest[0]    = cardinal_direction_45();
	scene_dest[1]    = cardinal_direction_45();
	scene_dest[2]    = cardinal_direction_45();
}

void vbl_scene_rotate_tl(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 45;
	scene_dest[1] = 45;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_t(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 45;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_tr(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);
	scene_dest[0] = 45;
	scene_dest[1] = -45;
	scene_dest[2] = 0;
}
void vbl_scene_rotate_cl(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 0;
	scene_dest[1] = 45;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_c(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);

	scene_dest[0] = 0;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_cr(void)
{
	if (debug_scene)

		printf("%s\n", __FUNCTION__);
	scene_dest[0] = 0;
	scene_dest[1] = -45;
	scene_dest[2] = 0;
}
void vbl_scene_rotate_bl(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);

	scene_dest[0] = -45;
	scene_dest[1] = 45;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_b(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);

	scene_dest[0] = -45;
	scene_dest[1] = 0;
	scene_dest[2] = 0;
}

void vbl_scene_rotate_br(void)
{
	if (debug_scene)
		printf("%s\n", __FUNCTION__);
	scene_dest[0] = -45;
	scene_dest[1] = -45;
	scene_dest[2] = 0;
}

void vbl_scene_spin_r(void)
{
	//scene_dest[1] += 90;
}
void vbl_scene_spin_l(void)
{
	//scene_dest[1] -= 90;
}
void vbl_scene_spin_u(void)
{
	scene_dest[0] += 90;
}
void vbl_scene_spin_d(void)
{
	scene_dest[0] -= 90;
}
