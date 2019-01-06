//
//  vbl_lissajous.h
//  vbl
//
//  Created by vs on 9/22/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_lissajous_h
#define vbl_lissajous_h

#include <stdio.h>

#include <r4/r4.h>

RLine* vbl_lissajous_create(double sigma, double omega, double step, int cycles);

void vbl_lissajous_test(void);

#endif /* vbl_lissajous_h */
