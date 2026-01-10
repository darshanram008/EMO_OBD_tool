/* 
 * File:   state_machine.h
 * Author: EMO energy
 *
 * Created on 7 August, 2023, 1:11 PM
 */

#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define GUI_disconnected 1
#define GUI_connected 2 
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "BMS_data_handle.h"
#include "GUI_data_handle.h"
#include "OBD_tool_events.h"
#include "flow_control.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/tmr2.h"
#include "mcc_generated_files/tmr0.h"

#include "xc.h"
//#include "pic18f27q84.h"

#include "mcc_generated_files/uart2.h"
    
#include "mcc_generated_files/memory.h"
#include "mcc_generated_files/mcc.h"

 extern  bool handshake; 
 extern uint8_t state;   
//includes
    


//global variables
    
extern uint8_t state;

    
    
//functions    
int current_state();


#ifdef	__cplusplus
}
#endif

#endif	/* STATE_MACHINE_H */

