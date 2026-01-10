/* 
 * File:   BMS_data_handle.h
 * Author: TRUE MIND COMPUTERS
 *
 * Created on 7 August, 2023, 6:15 PM
 */

#ifndef BMS_DATA_HANDLE_H
#define	BMS_DATA_HANDLE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "state_machine.h"   
extern uint8_t can_baud_eeprom;
extern uint16_t can_data1[8], can_data2[8], can_data3[8], can_data4[8], can_data5[8], can_data6[8], can_data7[8], can_data8[8], can_data9[8], can_data10[8];
extern volatile uint32_t id;
extern bool can_rx_receive_flag, status1, status2, status3, status4, status5, status6, status7, status8, status9, status10;
extern  uint16_t temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, cell_voltage[20];
extern uint16_t err1, err2, err3, pack_voltage, pack_current, SOC;
    
    
    
  
void CAN_main_data();
void canTransmit(uint32_t messageID, uint8_t data_length, uint8_t *TransmitData);


#ifdef	__cplusplus
}
#endif

#endif	/* BMS_DATA_HANDLE_H */

