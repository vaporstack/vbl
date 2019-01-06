//
//  vbl_cork.h
//  r4
//
//  Created by vs on 7/17/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_cork_h
#define vbl_cork_h

typedef struct RCorkRec
{
	void* src;
} RCorkRec;

RCorkRec* vbl_cork_load(const char* file);

#endif /* vbl_cork_h */
