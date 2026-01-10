/* 
 * File:   GUI_data_handle.h
 * Author: TRUE MIND COMPUTERS
 *
 * Created on 7 August, 2023, 4:22 PM
 */

#ifndef GUI_DATA_HANDLE_H
#define	GUI_DATA_HANDLE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "state_machine.h"
 
extern char command[150];
extern uint8_t rx_buffer[250];
extern volatile uint8_t rx_buffer_index;
extern volatile bool rx_receive_complete, boot_rx_receive_complete, OBD_reset ;

    
    

    void uart_transmit(char *uart_data);
    void command_service();
    void events();
    


#ifdef	__cplusplus
}
#endif

#endif	/* GUI_DATA_HANDLE_H */

