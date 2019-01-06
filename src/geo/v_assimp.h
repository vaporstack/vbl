//
//  v_assimp.h
//  vbl
//
//  Created by vs on 4/12/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef v_assimp_h
#define v_assimp_h

#include <r4/src/r4_config.h>

#ifdef R4_ENABLE_ASSIMP
#ifdef __APPLE__
	#define TEMPORARILY_ALLOW_TERRIBLE_LACK_OF_COMPARTMENTALIZATION_BETWEEN_LIBRARIES
#endif
//#else
//	#define DAssimpRec void
//#endif

#include <r4/src/render/r_render_assimp.h>

void v_render_assimp(DAssimpRec* rec);
void v_render_assimp_randomized(DAssimpRec* rec, double delta);
void v_render_assimp_lightbeam(DAssimpRec* rec, double delta);
void v_render_assimp_partial(DAssimpRec* rec, double delta);
void v_render_assimp_point_distance(DAssimpRec* rec, double x, double y, double z, double ir, double or);

#endif

#endif /* v_assimp_h */
