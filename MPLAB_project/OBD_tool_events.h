/* 
 * File:   OBD_tool_events.h
 * Author: TRUE MIND COMPUTERS
 *
 * Created on 7 August, 2023, 6:20 PM
 */

#ifndef OBD_TOOL_EVENTS_H
#define	OBD_TOOL_EVENTS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stdint.h"
    
extern long int start_point , end_point ;
extern uint16_t can_rx_conf[15], config_data[16],  can_rx_sd[64];
typedef enum {default_event,GUI_baud_rate_change_event, BMS_baud_rate_change_event, BMS_software_rst_event,BMS_get_config_event, BMS_upl_config_event,sd_data_download_event, BMS_bootloader_event}event;  
 event OBD_event;
    void events();


#ifdef	__cplusplus
}
#endif

#endif	/* OBD_TOOL_EVENTS_H */

