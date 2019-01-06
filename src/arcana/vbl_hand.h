//
//  vbl_hand.h
//  vbl
//
//  Created by vs on 7/2/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_hand_h
#define vbl_hand_h

#include <r4/src/geo/r_line.h>

RLine* vbl_hand_sketch(RLine* input);
int    vbl_hand_init(const char* path);
void   vbl_hand_deinit(void);

#endif /* vbl_hand_h */
