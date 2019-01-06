//
//  vbl_scene.h
//  vblit
//
//  Created by vs on 2/8/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_scene_h
#define vbl_scene_h

#include <stdio.h>

//extern int    camera_shake_hit;
//extern double camera_shake_amount;
typedef void (*funcptr_v_d)(double d);

void vbl_scene_camera_apply(void);
void vbl_scene_camera_unapply(void);

void vbl_scene_update(void);
void vbl_scene_init(void);
void vbl_scene_deinit(void);

void vbl_scene_adjust_screenrotate_x(double v);
void vbl_scene_adjust_screenrotate_y(double v);
void vbl_scene_adjust_screenrotate_z(double v);

void vbl_scene_velocity_set(double v);
double vbl_scene_velocity_get(void);

void vbl_scene_camera_shake_hit(void);
void vbl_scene_camera_shake_adjust(void);

void vbl_scene_rotate_random(void);
void vbl_scene_rotate_random_constrained(void);
void vbl_scene_rotate_reset(void);

void vbl_scene_rotate_front(void);
void vbl_scene_rotate_back(void);
void vbl_scene_rotate_top(void);
void vbl_scene_rotate_bottom(void);
void vbl_scene_rotate_left(void);
void vbl_scene_rotate_right(void);


void vbl_scene_rotate_tl(void);
void vbl_scene_rotate_t(void);
void vbl_scene_rotate_tr(void);

void vbl_scene_rotate_cl(void);
void vbl_scene_rotate_c(void);
void vbl_scene_rotate_cr(void);

void vbl_scene_rotate_bl(void);
void vbl_scene_rotate_b(void);
void vbl_scene_rotate_br(void);


void vbl_scene_spin_r(void);
void vbl_scene_spin_l(void);
void vbl_scene_spin_u(void);
void vbl_scene_spin_d(void);

void vbl_scene_draw_debug(void);

#endif /* vbl_scene_h */
