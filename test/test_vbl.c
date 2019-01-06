//
//  main.cpp
//  wash
//
//  Created by Andrew Macfarlane on 8/15/16.
//  Copyright © 2016 vaporstack. All rights reserved.
//

#include <r4/r4.h>
#include <stdio.h>

#include <stdlib.h>
#include <vbl/vbl.h>

#define TEST_FRAMES 256
typedef int (*test_fun)(void);

static int test_cascade(void)
{
	VCascade* c	= vbl_cascade_create();
	c->attrs.type      = VBL_FREQUENCY_FRAME;
	c->attrs.frequency = 10;

	for (int i = 0; i < TEST_FRAMES; i++)
	{
		vbl_cascade_update(c);
	}
	vbl_cascade_destroy(c);
	return 0;
}

static int test_tesselation(void)
{
	VTesselate* tess = v_tesselate_create(3, 6, 32.);
	v_tesselate_destroy(tess);
	return 0;
}

#define TNUM 2

int main(int argc, const char* argv[])
{

	printf("Running vbl tests.\n");
	printf("making sure vbl header matches library.\n");
	if (vbl_check_version_match(vbl_get_version_string()))
	{
		printf("version matches.\n");
	}
	else
	{
		printf("Version does not match!!\n");
		return 88;
	}

	test_tesselation();
	test_cascade();

	test_fun test[TNUM] = {test_tesselation, test_cascade};

	for (int i = 0; i < TNUM; i++)
	{
		int res = test[i]();
		printf("Test: %d of %d : %d\n", i, TNUM, res);

		return 0;
	}
}
