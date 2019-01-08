//
//  r_vec.c
//  r4
//
//  Created by Andrew Macfarlane on 1/7/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#include "v_block.h"

//#include "r_vec.h"
#include <r4/src/core/r_random.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
static V v_create(void)
{
	V* res = calloc(1,sizeof(V));
	res.v = vec3_create(NULL);

}
*/

vec3_list vec_list_create(unsigned long long num)
{
	vec3_list res = calloc(num, sizeof(vec3_list));
	for (int i = 0; i < num; i += 2)
	{
		res[i] = vec3_create(NULL);
	}
	return res;
}

int v_block_offset(VBlock* bloc, int x, int y, int z)
{
	int w   = bloc->width;
	int ind = (z * w * w) + (y * w) + x;

	if (ind > bloc->len)
	{
		printf("Invalid index!\n");
		return -1;
	}

	return ind;
}

//#ifdef DISALLOW_WHILE_VEC3_REFACTOR
//	debuggin RNG shit
static RRandom _custom_rng;
static bool    _custom_rng_initialized = false;

static void rvec_clear(VVec* vec)
{

	vec->x = 0;
	vec->y = 0;
	vec->z = 0;
}

VBlock* v_block_create(int width)
{
	// int x,y,z;

	VBlock* bloc = calloc(1, sizeof(VBlock));
	bloc->pos = 0;
	bloc->width = width;
	int width1  = width + 1;
	bloc->len   = width1 * width1 * width1;

	//bloc->data  = (vec3_t*)calloc(bloc->len, sizeof(vec3_list));
	//bloc->data2 = (vec3_t*)calloc(bloc->len, sizeof(vec3_list));
	//bloc->extra = (vec3_t*)calloc(bloc->len, sizeof(vec3_list));

	/*
	//for (int i = 0; i < bloc->len; ++i)
	//{

	bloc->data = vec_list2_create(bloc->len * 3);
	bloc->data2 = vec_list2_create(bloc->len * 3);
	bloc->extra = vec_list2_create(bloc->len * 3);
	//}
*/

	bloc->data  = calloc(bloc->len, sizeof(VVec));
	bloc->data2 = calloc(bloc->len, sizeof(VVec));
	bloc->extra = calloc(bloc->len, sizeof(VVec));

	for (int i = 0; i < bloc->len; i++)
	{
		bloc->data[i]  = calloc(1, sizeof(VVec));
		bloc->data2[i] = calloc(1, sizeof(VVec));
		bloc->extra[i] = calloc(1, sizeof(VVec));
		rvec_clear(bloc->data[i]);
		rvec_clear(bloc->data2[i]);
		rvec_clear(bloc->extra[i]);
	}
	return bloc;
}

void v_block_dist_grid(VBlock* bloc, double a, double b)
{
	float inc = (b - a) / ((float)bloc->width);
	printf("block spacing: %f\n", inc);

	for (int y = 0; y < bloc->width; ++y)
	{
		for (int x = 0; x < bloc->width; ++x)
		{
			for (int z = 0; z < bloc->width; ++z)
			{
				int ind = v_block_offset(bloc, x, y, z);

				vec3_t* v = &bloc->data[ind];
				if (v)
				{
					*v[0] = a + (x * inc);
					*v[1] = a + (y * inc);
					*v[2] = a + (z * inc);
				}
			}
		}
	}
}

static void randomSpherePoint(double x0, double y0, double z0, double radius,
			      double* x, double* y, double* z)
{
	double u     = r_rand_double(NULL);
	double v     = r_rand_double(NULL);
	double theta = 2 * M_PI * u;
	double phi   = acos(2 * v - 1);
	*x	   = x0 + (radius * sin(phi) * cos(theta));
	*y	   = y0 + (radius * sin(phi) * sin(theta));
	*z	   = z0 + (radius * cos(phi));
}

void v_block_dist_sphere(VBlock* bloc, double r)
{
	//	from:
	//	https://stackoverflow.com/questions/5531827/random-point-on-a-given-sphere/15048260#15048260

	double x, y, z;
	x = y = z = 0;
	for (int i = 0; i < bloc->len; ++i)
	{
		bool proceed = false;
		while (!proceed)
		{

			x = -.5 + r_rand_double(NULL);
			y = -.5 + r_rand_double(NULL);
			z = -.5 + r_rand_double(NULL);
			if (x * x + y * y + z * z <= 1)
				proceed = true;
		}

		double d = sqrt(x * x + y * y + z * z);

		x /= d;
		y /= d;
		z /= d;

		x *= r;
		y *= r;
		z *= r;

		// randomSpherePoint(0,0,0,r, &x, &y, &z );
		v_block_set_index(bloc, i, x, y, z);
	}
}
void v_block_dist_sphere_vol(VBlock* bloc, double r)
{
}

void v_block_dist_rand(VBlock* bloc, double a, double b)
{
	/*
	double delta = b - a;
	// float inc = (b-a) / ((float)bloc->width);
	// printf("block spacing: %f\n", inc);
	if (!_custom_rng_initialized)
	{
		_custom_rng = r_rand_create();
	}
	for (int y = 0; y < bloc->width; ++y)
	{
		for (int x = 0; x < bloc->width; ++x)
		{
			for (int z = 0; z < bloc->width; ++z)
			{
				int ind = v_block_offset(bloc, x, y, z);

				vec3_t v = bloc->data[ind];
				if (v)
				{

					v[0] =
					    a + (r_rand_double(&_custom_rng) *
						 delta);
					v[1] =
					    a + (r_rand_double(&_custom_rng) *
						 delta);
					v[2] =
					    a + (r_rand_double(&_custom_rng) *
						 delta);
				}
			}
		}
	}*/
}

void v_block_destroy(VBlock* bloc)
{

	free(bloc->data);
	free(bloc->extra);
	free(bloc);
}

static bool errorcheck(VBlock* b, int x, int y, int z)
{
	return (x > b->width || y > b->width || z > b->width || x < 0 || y < 0 || z < 0);
}

VVec* v_block_get(VBlock* block, int x, int y, int z)
{
	if (errorcheck(block, x, y, z))
	{
		printf("Unable to access block outside width!\n");
		return NULL;
	}

	return block->data[v_block_offset(block, x, y, z)];
}

VVec* v_block_get_data(VBlock* block, int x, int y, int z)
{
	if (errorcheck(block, x, y, z))
	{
		printf("Unable to access block outside width!\n");
		return NULL;
	}
	return block->data[v_block_offset(block, x, y, z)];
}

VVec* v_block_get_data2(VBlock* block, int x, int y, int z)
{
	if (errorcheck(block, x, y, z))
	{
		printf("Unable to access block outside width!\n");
		return NULL;
	}
	return block->data2[v_block_offset(block, x, y, z)];
}

VVec* v_block_get_extra(VBlock* bloc, int x, int y, int z)
{
	if (x > bloc->width || y > bloc->width || z > bloc->width)
	{
		printf("Unable to access block outside width!\n");
		return NULL;
	}
	return bloc->extra[v_block_offset(bloc, x, y, z)];
}

void v_block_set_vec(VBlock* bloc, int x, int y, int z, vec3_t* vec)
{
	int ind = v_block_offset(bloc, x, y, z);
	//	int ind = (( x * y ) + z );
	VVec* point = bloc->data[ind];

	point->x = *vec[0];
	point->y = *vec[1];
	point->z = *vec[2];
}

void v_block_set_data(VBlock* bloc, int x, int y, int z, vec3_t vec)
{
	int ind = v_block_offset(bloc, x, y, z);
	//	int ind = (( x * y ) + z );
	VVec* point = bloc->data[ind];

	point->x = vec[0];
	point->y = vec[1];
	point->z = vec[2];
}

void v_block_set_data2(VBlock* bloc, int x, int y, int z, vec3_t vec)
{
	int ind = v_block_offset(bloc, x, y, z);
	//	int ind = (( x * y ) + z );
	VVec* point = bloc->data2[ind];

	point->x = vec[0];
	point->y = vec[1];
	point->z = vec[2];
}

void v_block_set_extra(VBlock* bloc, int x, int y, int z, vec3_t vec)
{
	int   ind   = v_block_offset(bloc, x, y, z);
	VVec* point = bloc->extra[ind];

	point->x = vec[0];
	point->y = vec[1];
	point->z = vec[2];
}

void v_block_set(VBlock* bloc, int px, int py, int pz, double x, double y, double z)
{
	int   ind   = v_block_offset(bloc, px, py, pz);
	VVec* point = bloc->data[ind];

	//VVec* point = &list[ind];

	point->x = x;
	point->y = y;
	point->z = z;
}

void v_block_setdata2(VBlock* bloc, int px, int py, int pz, double x, double y, double z)
{
	int   ind   = v_block_offset(bloc, px, py, pz);
	VVec* point = bloc->data2[ind];

	//VVec* point = &list[ind];

	point->x = x;
	point->y = y;
	point->z = z;
}

void v_block_setextra(VBlock* bloc, int px, int py, int pz, double x, double y, double z)
{
	int   ind   = v_block_offset(bloc, px, py, pz);
	VVec* point = bloc->extra[ind];

	//VVec* point = &list[ind];

	point->x = x;
	point->y = y;
	point->z = z;
}

void v_block_set_index(VBlock* block, int index, double x, double y, double z)
{
	/*
	// vec3_t dst = block->data[index];
	block->data[index][0] = x;
	block->data[index][1] = y;
	block->data[index][2] = z;
	// block->data[index] = vec;
*/
}

void v_block_set_index_vec(VBlock* block, int index, vec3_t* vec)
{
	/*
	vec3_t* dst = &block->data[index];
	dst[0]      = vec[0];
	dst[1]      = vec[1];
	dst[2]      = vec[2];

	// block->data[index] = vec;
	*/
}

#define TMP_BLOCK_SIZE 8
void v_block_test()
{
	VBlock* block_src = v_block_create(TMP_BLOCK_SIZE);
	VBlock* block_dst = v_block_create(TMP_BLOCK_SIZE);
	vec3_t  vec   = vec3_create(NULL);
	vec[0]	= 20.0f;
	vec[1]	= 12.0f;
	vec[2]	= 8.0f;
	v_block_set_vec(block_src, 2, 2, 2, &vec);

	// Vec3* vec3 = v_block_get(block, 2,2,2);
	v_block_destroy(block_src);
}

//#endif
