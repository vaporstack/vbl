//
//  vbl_etherdream.h
//  vbl
//
//  Created by vs on 5/30/19.
//  Copyright © 2019 vaporstack. All rights reserved.
//

#ifndef vbl_etherdream_h
#define vbl_etherdream_h

void vbl_etherdream_init(void);
void vbl_etherdream_deinit(void);
void vbl_etherdream_test_all(int mode);
void vbl_etherdream_test(int which, int mode);
int  vbl_etherdream_count(void);
void vbl_etherdream_draw_rline(void* line);
void vbl_etherdream_queue_rline(void* line);

#endif /* vbl_etherdream_h */
