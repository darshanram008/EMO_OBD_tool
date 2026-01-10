/* 
 * File:   flow_control.h
 * Author: TRUE MIND COMPUTERS
 *
 * Created on 7 August, 2023, 6:47 PM
 */

#ifndef FLOW_CONTROL_H
#define	FLOW_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "stdbool.h"
//  extern uint8_t can_ack[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//  extern uint8_t sd_data[8], can_rx_sd_byte;
extern uint8_t  can_tx_conf[22], can_tx[8];
extern uint16_t can_rx_conf[15],config_data[16],  can_rx_sd[64];
extern bool timeout2, timeout0;
    void sd_data_flow();
    void waitForNextCANMsg();
    void waitForNextGUIMsg();
void B2G_config_data_flow();

#ifdef	__cplusplus
}
#endif

#endif	/* FLOW_CONTROL_H */

