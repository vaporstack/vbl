//
//  vbl_input_logger.h
//  vbl
//
//  Created by vs on 10/19/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#ifndef vbl_input_logger_h
#define vbl_input_logger_h

#define VBLIT_ENABLE_INPUTLOGGER

#ifdef VBLIT_ENABLE_INPUTLOGGER

typedef enum VBLInputLoggerMonomeType
{
	VBL_INPUT_LOGGER_MONOME_TYPE_TAP,
	VBL_INPUT_LOGGER_MONOME_TYPE_PRESS,
	VBL_INPUT_LOGGER_MONOME_TYPE_HOLD
}VBLInputLoggerMonomeType;

typedef enum VBLInputLoggerTwistertype
{
	VBL_INPUT_LOGGER_TWISTER_TYPE_TAP,
	VBL_INPUT_LOGGER_TWISTER_TYPE_TWIST
}VBLInputLoggerTwisterType;

typedef struct VBLInputLogger
{
	int num;
	
}VBLInputLogger;

#include <stdio.h>

VBLInputLogger*  vbl_input_logger_create(double session_start_time, const char* log_path);
void vbl_inputlogger_destroy(VBLInputLogger* logger);

void vbl_inputlogger_log_monome(VBLInputLogger* logger, const char* ident, int x, int y, int type);
void vbl_inputlogger_log_keypress(VBLInputLogger* logger, int key, int mods, int state);
void vbl_inputlogger_log_mousepress(VBLInputLogger* logger, int button, int state);
void vbl_inputlogger_log_mousemove(VBLInputLogger* logger, double x, double y);
void vbl_inputlogger_log_twister(VBLInputLogger, int page, int type, int v);


#endif

#endif /* vbl_input_logger_h */
