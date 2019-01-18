//
//  vbl_hub.h
//  vbl
//
//  Created by vs on 4/25/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_hub_h
#define vbl_hub_h

#define VBL_NUM_BANKS 4
#define VBL_NUM_PAGES 4

#define VBL_NUM_ROUTINES VBL_NUM_BANKS* VBL_NUM_PAGES * 2
#define VBL_3D_THRESHOLD 7

enum VBL_COLORCYCLE_MODES
{
	VBL_COLORCYCLE_NONE,
	VBL_COLORCYCLE_UP,
	VBL_COLORCYCLE_DOWN,
	VBL_COLORCYCLE_SIN,
	VBL_COLORCYCLE_COS,
	VBL_COLORCYCLE_TAN,
	VBL_COLORCYCLE_END
};

enum VBL_WAVEDATA_SOURCES
{
	VBL_WAVEDATA_SQR,
	VBL_WAVEDATA_SAW,
	VBL_WAVEDATA_TRI,
	VBL_WAVEDATA_SIN,
	VBL_WAVEDATA_COS,
	VBL_WAVEDATA_TAN,
	//VBL_WAVEDATA_ATN,
	VBL_WAVEDATA_MAX
};

typedef struct VblBank
{
	int id;
	int routine;
	int flipx;
	int flipy;
	int intensity;
	int fill;
	int color;
	int bang;
	int extra;
	int colorcycle;
} VblBank;

typedef struct VblData
{
	double  wav1;
	double  wav2;
	double  wav3;
	double  wav4;
	double  hit1;
	double  hit2;
	double  hit3;
	double  hit4;
	double  t;
	double  t2;
	double  t4;
	double  wsqr;
	double  wsaw;
	double  wtri;
	double  wsin;
	double  wcos;
	double  wtan;
	double* mut1;
	double* mut2;
	double* mut3;
	double* mut4;
} VblData;

typedef struct VblHub
{
	int debug;
	int bank;
	int page;
	int audio;
	int mode3d;

	double position;
	double speed;
	double speed_mult;
	double audio1;
	double audio2;
	double audio3;

	int wave1;
	int wave2;
	int wave3;
	int wave4;

	int hit1;
	int hit2;
	int hit3;
	int hit4;

	int mut1;
	int mut2;
	int mut3;
	int mut4;

	int    artist;
	int    overdrive;
	double playback_speed;

	VblBank banks[VBL_NUM_BANKS];
	int     assignments[VBL_NUM_BANKS];

	VblData d;

} VblHub;

void vbl_hub_init(VblHub* hub);
void vbl_hub_deinit(VblHub* hub);

void vbl_hub_reset(VblHub* hub);

void vbl_hub_cycle_mut1(VblHub* hub);
void vbl_hub_cycle_mut2(VblHub* hub);
void vbl_hub_cycle_mut3(VblHub* hub);
void vbl_hub_cycle_mut4(VblHub* hub);

void vbl_hub_toggle_bank_fill(VblHub* hub);
void vbl_hub_toggle_bank_flipx(VblHub* hub);
void vbl_hub_toggle_bank_flipy(VblHub* hub);
void vbl_hub_toggle_bank_colorcycle(VblHub* hub);
void vbl_hub_toggle_bank_color(VblHub* hub);

void vbl_hub_switch_artist(VblHub* hub);
void vbl_hub_toggle_overdrive(VblHub* hub);
void vbl_hub_toggle_auto(VblHub* hub);

void vbl_hub_flip_sequencer_at_current_playhead(VblHub* hub);
void vbl_hub_toggle_mode3d(VblHub* hub);
void vbl_hub_flip_sequencer_at_xy(VblHub* hub, int x, int y);

void vbl_hub_sequencer_reset(VblHub* hub);

void vbl_hub_unserialize(VblHub* hub, const char* path);
void vbl_hub_serialize(VblHub* hub, const char* path);
void vbl_hub_fire_hit(VblHub* hub, int which, int val);

#endif /* vbl_hub_h */
