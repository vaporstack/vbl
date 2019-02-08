//
//  monome.h
//  vblit
//
//  Created by Andrew Macfarlane on 1/11/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_vbl_h_
#define v_vbl_h_

#define VBL_VERSION "0.0.2"
#define VBL_VERSION_MAJOR 0
#define VBL_VERSION_MINOR 0
#define VBL_VERSION_PATCH 2

#define VBL_VERSION_CHECK(maj, min) ((maj == VBL_VERSION_MAJOR) && (min <= VBL_VERSION_MINOR))


int   vbl_check_version_match(const char* str);
int   vbl_check_compat_match(const char* str);
char* vbl_get_version_string(void);
char* vbl_get_compat_string(void);

#include <stdlib.h>
#include <stdio.h>

static inline char* vbl_get_compat_string_header(void)
{
	char* buf = (char*)calloc(256, sizeof(char));
	sprintf(buf, "%d.%d", VBL_VERSION_MAJOR, VBL_VERSION_MINOR);
	return buf;
}

static inline char* vbl_get_version_string_header(void)
{
	char* buf = (char*)calloc(256, sizeof(char));
	sprintf(buf, "%d.%d.%d", VBL_VERSION_MAJOR, VBL_VERSION_MINOR,
		VBL_VERSION_PATCH);
	return buf;
}

//#include "src/geo/v_assimp.h"
#include "src/anim/v_behave.h"
#include "src/anim/v_swarm.h"
#include "src/anim/v_tracer.h"
#include "src/anim/vbl_scene.h"
#include "src/arcana/vbl_hand.h"
#include "src/core/vbl_hub.h"
#include "src/comms/v_channel.h"
#include "src/control/vbl_cascade.h"
#include "src/control/vbl_pad.h"
#include "src/geo/v_block.h"
#include "src/geo/v_primitives.h"
#include "src/geo/v_tesselate.h"
#include "src/geo/v_vec.h"
#include "src/geo/vbl_qhull.h"
#include "src/plugin/v_plugins.h"
#include "src/particle/vbl_particlesystem_mgr.h"
#include "src/seq/v_mark.h"
#include "src/seq/v_seq.h"
#include "src/seq/v_track.h"
#include "src/study/v_bpm.h"
#include "src/time/v_clock.h"
#include "src/time/v_stepseq.h"
#include "src/util/v_serial.h"
#include "src/util/v_trigger.h"
#include "src/util/v_unit.h"
#include "src/vbl_types.h"


void vbl_init(void);
void vbl_deinit(void);

/*

 enum VBLParticlePluginSpawnerFrequency
 {
 VBL_PARTICLEPLUGIN_SPAWNERFREQ_FRAME,
 VBL_PARTICLEPLUGIN_SPAWNERFREQ_TIME,
 VBL_PARTICLEPLUGIN_SPAWNERFREQ_REUSE,
 VBL_PARTICLEPLUGIN_SPAWNERFREQ_NONE
 };




 */

#endif /* sequencer_h_ */
