//
//  v_trigger.c
//  vbl
//
//  Created by vs on 7/12/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include "v_trigger.h"
#include <stdlib.h>

VTriggerList* v_trigger_list_create(long num)
{
	VTriggerList* list = calloc(1, sizeof(VTriggerList));
	list->num = num;
	list->triggers = (VTrigger*)calloc(num, sizeof(VTrigger*));
	return list;
}

void	v_trigger_list_destroy(VTriggerList* list)

{
	free(list);
}
