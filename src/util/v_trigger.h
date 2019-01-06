//
//  v_trigger.h
//  vbl
//
//  Created by vs on 7/12/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef v_trigger_h
#define v_trigger_h


typedef void (*VTrigger)(void);

typedef struct VTriggerList
{
	VTrigger** triggers;
	long num;
} VTriggerList;

VTriggerList* v_trigger_list_create(long num);
void	v_trigger_list_destroy(VTriggerList* list);


#endif /* v_trigger_h */
