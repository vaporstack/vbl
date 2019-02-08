
//  gui.c
//  gui
//
//  Created by Andrew Macfarlane on 11/06/17.
//  Copyright © 2017 vaporstack. All rights reserved.

#include "vbl.h"



int vbl_check_version_match(const char* header_generated_version)
{
	char* version =vbl_get_version_string();
	int matches = (0 == strcmp(version, header_generated_version));
	free(version);
	return matches;
}

char* vbl_get_version_string(void)
{
	char* buf = calloc(256, sizeof(char));
	sprintf(buf, "%d.%d.%d", VBL_VERSION_MAJOR, VBL_VERSION_MINOR,
		VBL_VERSION_PATCH);
	return buf;
}

void vbl_init(void)
{
	vbl_hand_init("/Users/vs/art/barn/src/art/wash/org/components/shading/hatching_2-2018_3_4-22_44_5.wash");
}

void vbl_deinit(void)
{
	vbl_hand_deinit();
}
