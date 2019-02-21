//
//  vbl_particleplugin_emitter.h
//  vbl
//
//  Created by vs on 12/6/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_particleplugin_emitter_h
#define vbl_particleplugin_emitter_h

#include "vbl_particleplugin.h"
#include "vbl_particlesystem.h"

typedef void (*generic_void_pointer)(void);



enum VBLParticlePluginEmitterType
{
	VBL_PARTICLEPLUGIN_EMITTYPE_POINT,
	VBL_PARTICLEPLUGIN_EMITTYPE_BOX,
	VBL_PARTICLEPLUGIN_EMITTYPE_SPHERE,
	VBL_PARTICLEPLUGIN_EMITTYPE_NONE
};

enum VBLParticlePluginEmitterFrequency
{
	VBL_PARTICLEPLUGIN_EMITFREQ_FRAME,
	VBL_PARTICLEPLUGIN_EMITFREQ_TIME,
	VBL_PARTICLEPLUGIN_EMITFREQ_TRIGGER,
	//	VBL_PARTICLEPLUGIN_EMITFREQ_REUSE,
	VBL_PARTICLEPLUGIN_EMITFREQ_NONE
};

enum VBLParticlePluginEmitterDensity
{
	VBL_PARTICLEPLUGIN_EMITDENSITY_ONE,
	VBL_PARTICLEPLUGIN_EMITDENSITY_FIXED,
	VBL_PARTICLEPLUGIN_EMITDENSITY_RANDOM,
	VBL_PARTICLEPLUGIN_EMITDENSITY_NONE
};

enum VBLParticlePluginEmitterDirection
{
	VBL_PARTICLEPLUGIN_EMITDIR_NONE,
	VBL_PARTICLEPLUGIN_EMITDIR_DIRECTIONAL,
	VBL_PARTICLEPLUGIN_EMITDIR_CONE,
	VBL_PARTICLEPLUGIN_EMITDIR_OUTWARDS
};

#include <stdbool.h>

typedef struct VPPSEmitInfo
{
	//unsigned (*spawn_num)(VParticlePlugin* plug, VParticleSystem* sys);
	void (*place)(VParticlePlugin* plug, VParticleSystem* sys, double* x, double* y, double* z);
	bool (*check)(struct VPPSEmitInfo* info);
	//void (*setvel)(VParticlePlugin* plug, VParticle* p);

	unsigned (*density)(struct VPPSEmitInfo* info);
	unsigned int	  spawn_type;
	unsigned int	  spawn_freq;
	unsigned int	  spawn_density;
	unsigned int	  emit_dir;
	unsigned int spawn_volume;
	unsigned	      density_n;
	double		      last;
	double		      frequency;
	bool		      reuse;
	bool		      inside;
	double		      ox, oy, oz;
	double		      bx, by, bz;
	double		      dx, dy, dz;
	double		      vx, vy, vz;
	double		      r;
	generic_void_pointer  trigger;
	generic_void_pointer* triggers;
	unsigned	      trigger_num;
	//unsigned int bounds_behavior;
} VPPSEmitInfo;

VPPSEmitInfo*    vbl_particleplugin_emitterinfo_create(void);
VParticlePlugin* vbl_particleplugin_emitter_create(VPPSEmitInfo* info);
void		 vbl_particleplugin_emitter_add_trigger(VParticlePlugin* plug, generic_void_pointer trigger);
void		 vbl_particleplugin_emitter_emit(VParticlePlugin* plug, void* sys);
VParticlePlugin* vbl_particleplugin_emitter_create_point(void);
VParticlePlugin* vbl_particleplugin_emitter_create_box(double x, double y, double z);
void		vbl_particleplugin_emitter_reset(VParticlePlugin* plug, VParticleSystem* sys, VParticle* p);
#endif /* vbl_particleplugin_emitter_h */
