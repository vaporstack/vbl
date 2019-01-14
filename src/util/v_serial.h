//
//  v_serial.h
//  vbl
//
//  Created by Andrew Macfarlane on 4/22/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef v_serial_h
#define v_serial_h

//#include <r4/src/r4_config.h>
//#include "../seq/v_seq.h"

void*       vbl_read_file_as_bin(const char* path, long* len);
const char* vbl_read_file_as_text(const char*);
char*       vbl_read_file_as_text_nc(const char*);
int	 vbl_write_text_to_file(const char*, const char*);

#ifndef RPLATFORM_IOS
#include <cjson/cJSON.h>

const char* v_serialize_sequence_json(void* data);
void*       v_unserialize_sequence_json(const char* path);

#endif

#endif /* v_serial_h */
