#ifndef _GSM_MODULE_H
#define _GSM_MODULE_H

#include "Arduino.h"
#include "OS/OSThreadKernel.h"

/*!
*   @brief Function declarations. 
*/
void start_gsm_module(void); 
void spinlock_gsm(void);

#endif 