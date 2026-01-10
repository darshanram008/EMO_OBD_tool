#include "OBD_tool_events.h"
#include "state_machine.h"
#include "flow_control.h"
#include "math.h"
uint8_t BMS_bootloader_packet_num;
uint8_t BMS_bootloader_string_array[200];
uint8_t msb, lsb;
long int start_point = 0, end_point = 0;
uint8_t can_timeOut[8] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t can_ack[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t can_done[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t can_sft_rst[4] = {0x00, 0xFF, 0x01, 0x00};
uint8_t can_data_req[4] = {0x04, 0x0D, 0x0F, 0x63};
uint8_t Tx_baud_500[4] = {0x04, 0x0D, 0x0F, 0x64};
uint8_t Tx_baud_250[4] = {0x04, 0x0D, 0x0F, 0x65};
uint8_t Tx_baud_125[4] = {0x04, 0x0D, 0x0F, 0x66};
uint8_t BMS_ID_req[4] = {0x04, 0x0D, 0x0F, 0x67};
uint8_t can_config_req[4] = {0x00, 0x01, 0x00, 0x00};
uint16_t can_rx_conf[15], config_data[16], can_rx_sd[64];
char transmit1[150], transmit2[150], transmit3[150], transmit4[80], transmit5[300];


 char timeout_msg[] = "AT+TIME\r\n", gui_ack[10] = "AT+DONE\r\n", AT_error[] = "AT+ERRR\r\n";
uint8_t sd_data[8], can_rx_sd_byte;

void events() {
    uint8_t BMS_bootloader_data[8];
    
    int m = 0, k = 0, i = 0, count = 0, j;
    enum OBD_event;
    uint32_t BMS_ID, firmware_loc;
    uint8_t firmware_ver, bootloader_ver;
    char bootloader_string[100];

    switch (OBD_event) {
        case default_event:
             IO_RB0_SetHigh();
             IO_RC5_SetLow();
            canTransmit(0x1806F4F4, 4U, can_data_req);
            CAN_main_data();

            sprintf(transmit1, "VOLTSOC%u,%u,%u,%u,%u,%uVOLTSOC\r\n", err1, pack_voltage, pack_current, SOC, err2, err3);
            sprintf(transmit2, "CELLVOL%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uCELLVOL\r\n", cell_voltage[0], cell_voltage[1], cell_voltage[2], cell_voltage[3], cell_voltage[4], cell_voltage[5], cell_voltage[6], cell_voltage[7], cell_voltage[8], cell_voltage[9], cell_voltage[10], cell_voltage[11], cell_voltage[12], cell_voltage[13], cell_voltage[14], cell_voltage[15], cell_voltage[16], cell_voltage[17], cell_voltage[18], cell_voltage[19]);            
            sprintf(transmit3, "THERMIS%u,%u,%u,%u,%u,%uTHERMIS\r\n", temperature1, temperature2, temperature3, temperature4, temperature5, temperature6);
           
            
            //UART2_Write(2);
            uart_transmit(transmit1);
            uart_transmit(transmit2);
            uart_transmit(transmit3);
            break;
        case GUI_baud_rate_change_event:
            IO_RB0_SetLow();
            IO_RC5_SetHigh();
            count = 0;

            for (i = start_point; i < end_point; i++) {
                count++;
            }
            if (count == 9) {

                switch (command[start_point + 8]) {
                    case '1': //9600
                        GUI_baud_rate = 1;
                        UART2_Initialize();
                       
                        handshake = false;
                        state = GUI_disconnected;
                        break;
                    case '2': //57600
                       
                        GUI_baud_rate = 2;
                        UART2_Initialize();
                        handshake = false;
                        state = GUI_disconnected;
                        break;
                    case '3': //115200
                       GUI_baud_rate = 3;
                        UART2_Initialize();
                     
                        //DATAEE_WriteByte(0x00, 0x03);
                        handshake = false;
                        state = GUI_disconnected;
                        break;
                    default:
                        uart_transmit(AT_error);
                        break;


                }

            } else {
                uart_transmit(AT_error);

            }
            break;

        case BMS_baud_rate_change_event:
             IO_RB0_SetLow();
             IO_RC5_SetHigh();

            count = 0;

            for (i = start_point; i < end_point; i++) {
                count++;
            }
            if (count == 9) {

                switch (command[(start_point + 8)]) {
                    case '1': //125kbps
                        canTransmit(0x1806F4F4, 4U, Tx_baud_125);
                        DATAEE_WriteByte(0x00, 0x01);
                        CAN1_Initialize();
                        break;
                    case '2': //250kbps    
                        canTransmit(0x1806F4F4, 4U, Tx_baud_250);
                        DATAEE_WriteByte(0x00, 0x02);
                        CAN1_Initialize();
                        break;
                    case '3': //500kbps
                        canTransmit(0x1806F4F4, 4U, Tx_baud_500);
                        DATAEE_WriteByte(0x00, 0x03);
                        CAN1_Initialize();
                        break;
                    default:
                        uart_transmit(AT_error);
                        break;

                }


            } else {
                uart_transmit(AT_error);

            }
            break;
        case sd_data_download_event:
             IO_RB0_SetLow();
             IO_RC5_SetHigh();
            count = 0;
            k = 0;

            for (i = start_point; i < end_point; i++) {
                count++;
            }
            if ((count == 27) &&(command[start_point + 8] == '1')) {

                char* token = strtok(command, ",");
                char *ptr;
                while (token != NULL) {

                    sd_data[k] = strtol(token, &ptr, 10);
                    k++;

                    token = strtok(NULL, ",");

                }
                can_tx[0] = 'S';
                can_tx[1] = 'D';
                can_tx[2] = sd_data[1];
                can_tx[3] = sd_data[2];
                can_tx[4] = sd_data[3];
                can_tx[5] = sd_data[4];
                can_tx[6] = sd_data[5];
                can_tx[7] = sd_data[6];

                canTransmit(0x1806F4F4, 8U, can_tx);

                sd_data_flow();


            } else if ((count == 9) &&(command[start_point + 8] == '2')) {

                can_tx[0] = 'S';
                can_tx[1] = 'D';
                can_tx[2] = 0xFF;
                can_tx[3] = 0xFF;
                can_tx[4] = 0x00;
                can_tx[5] = 0x00;
                can_tx[6] = 0x00;
                can_tx[7] = 0x00;

                canTransmit(0x1806F4F4, 8U, can_tx);


                sd_data_flow();

            } else {
                uart_transmit(AT_error);

            }
            break;
        case BMS_software_rst_event:
             IO_RB0_SetLow();
             IO_RC5_SetHigh();
            can_rx_receive_flag = false;

            count = 0;

            for (i = start_point; i < end_point; i++) {
                count++;
            }
            if (count == 9) {

                switch (command[(start_point + 8)]) {
                    case '1':
                        canTransmit(0x1806F4F5, 4U, can_sft_rst);
                        break;
                }


                waitForNextCANMsg();

                if ((status8 == true) &&(can_data8[0] == 0x01)) {

                    uart_transmit(gui_ack);

                } else {
                    uart_transmit(timeout_msg);
                    canTransmit(0x1806F4F6, 8U, can_timeOut);
                }

                status8 = false;
                can_rx_receive_flag = false;

            } else {
                uart_transmit(AT_error);

            }
            break;

        case BMS_get_config_event:
             IO_RB0_SetLow();
             IO_RC5_SetHigh();
            if (command[start_point + 8] == '2') {
                canTransmit(0x1806F4F5, 4U, can_config_req);
                TMR2_Start();
                do {

                    if (timeout2 == true) {
                        uart_transmit(timeout_msg);
                        canTransmit(0x1806F4F6, 8U, can_timeOut);
                        timeout2 = false;
                        break;

                    }

                } while (status9 != true);

                if (status9 == true) {
label:
                    switch (can_data9[0]) {
                        case 0x01:
                            can_rx_conf[0] = (can_data9[2] << 8) | can_data9[1];
                            can_rx_conf[1] = (can_data9[4] << 8) | can_data9[3];
                            can_rx_conf[2] = (can_data9[6] << 8) | can_data9[5];
                            can_rx_conf[3] = can_data9[7];
                            canTransmit(0x1806F4F6, 4U, can_ack);
                            status9 = false;
                            can_rx_receive_flag = false;
                            break;


                        case 0x02:
                            can_rx_conf[4] = (can_data9[2] << 8) | can_data9[1];
                            can_rx_conf[5] = (can_data9[4] << 8) | can_data9[3];
                            can_rx_conf[6] = (can_data9[6] << 8) | can_data9[5];
                            can_rx_conf[7] = can_data9[7];
                            canTransmit(0x1806F4F6, 4U, can_ack);
                            status9 = false;
                            can_rx_receive_flag = false;
                            break;


                        case 0x03:
                            can_rx_conf[8] = (can_data9[2] << 8) | can_data9[1];
                            can_rx_conf[9] = can_data9[3];
                            can_rx_conf[10] = can_data9[4];
                            can_rx_conf[11] = can_data9[5];
                            can_rx_conf[12] = can_data9[6];
                            can_rx_conf[13] = can_data9[7];
                            canTransmit(0x1806F4F6, 4U, can_ack);
                            status9 = false;
                            can_rx_receive_flag = false;
                            break;


                        case 0x04:
                            can_rx_conf[14] = can_data9[1];
                            canTransmit(0x1806F4F6, 4U, can_ack);
                            status9 = false;
                            can_rx_receive_flag = false;
                            break;

                        default:
                            status9 = false;
                            can_rx_receive_flag = false;
                            break;


                    }
                    waitForNextCANMsg();
                    //                        TMR2_Start(); //wait for next message from BMS
                    //                        do {
                    //
                    //                            if (timeout == true) {
                    //                                uart_transmit(timeout_msg);
                    //                                canTransmit(0x1806F4F6, 8U, can_timeOut);
                    //                                timeout = false;
                    //                                break;
                    //                            }
                    //                        } while (can_rx_receive_flag != true);
                    //                        TMR2_Stop();
                    if (status9 == true) {
                        goto label;
                    } else if ((status8 == true) &&(can_data8[0] == 0x02)) {

                        sprintf(transmit4, "CONFIGR%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uCONFIGR\r\n", can_rx_conf[0], can_rx_conf[1], can_rx_conf[2], can_rx_conf[3], can_rx_conf[4], can_rx_conf[5], can_rx_conf[6], can_rx_conf[7], can_rx_conf[8], can_rx_conf[9], can_rx_conf[10], can_rx_conf[11], can_rx_conf[12], can_rx_conf[13], can_rx_conf[14]);
                        uart_transmit(transmit4);

                    } else {
                        uart_transmit(timeout_msg);
                        canTransmit(0x1806F4F6, 8U, can_timeOut);
                    }

                } else {
                    uart_transmit(timeout_msg);
                    canTransmit(0x1806F4F6, 8U, can_timeOut);

                }

                status8 = false;
                can_rx_receive_flag = false;

            } else {
                uart_transmit(AT_error);
            }

            break;
        case BMS_upl_config_event:
             IO_RB0_SetLow();
             IO_RC5_SetHigh();

            j = 0;
            k=0;
            if (command[start_point + 8] == '1') {

                char* token = strtok(command, ",");
                char *ptr;
                while (token != NULL) {

                    config_data[k] = strtol(token, &ptr, 10); //CONVERT STRING TO INTEGER BASE 10
                    k++;

                    token = strtok(NULL, ",");

                }

                for (i = 1; i < 16; i++) {
                    if (i <= 7) //number of params with 2 bytes data (4 digits))
                    {

                        msb = config_data[i] >> 8;
                        lsb = config_data[i] & 0x00FF;
                        can_tx_conf[2 * j + 1] = msb;
                        can_tx_conf[2 * j] = lsb;

                    } else if (i > 7) //remaianing paramas with 1 byte data(2 digits)
                    {
                        can_tx_conf[j + 7] = config_data[i];
                    }
                    j++;
                }

                // uart_transmit(gui_ack);

                B2G_config_data_flow();

            } else {
                uart_transmit(AT_error);
            }
            break;

        case BMS_bootloader_event:
             IO_RB0_SetLow();
             IO_RC5_SetHigh();

            can_rx_receive_flag = false;

            count = 0;

            for (i = start_point; i < end_point; i++) {
                count++;
            }
            if ((count == 9) && (command[(start_point + 8)] == '1')) 
            {

                label3:
                switch (command[(start_point + 8)]) {
                    case '1':
                        canTransmit(0x1806F4F5, 4U, can_sft_rst);
                        waitForNextCANMsg();
                        if ((status8 == true) &&(can_data8[0] == 0x01)) {

                            uart_transmit(gui_ack);
                            waitForNextGUIMsg();
                            OBD_event = default_event;
                            command_service();
                            if (OBD_event == BMS_bootloader_event) {

                                OBD_event = default_event;
                                status8 = false;
                                can_rx_receive_flag = false;
                                goto label3;

                            } else {
                                uart_transmit(AT_error);
                            }


                        } else {
                            uart_transmit(timeout_msg);
                            canTransmit(0x1806F4F6, 8U, can_timeOut);
                        }
                        status8 = false;
                        can_rx_receive_flag = false;



                        break;
                    case '2':
                        canTransmit(0x1806F4F4, 4U, BMS_ID_req);
label4:
                        waitForNextCANMsg();
                        if (status9 != true) {

                            uart_transmit(timeout_msg);
                            canTransmit(0x1806F4F6, 8U, can_timeOut);
                            break;

                        }

                        switch (can_data9[0]) {
                            case 0xF1:


                                BMS_ID = (uint32_t)can_data9[4] << 24 | (uint24_t)can_data9[3] << 16 | can_data9[2] << 8 | can_data9[1];
                                firmware_ver =  can_data9[5];
                                bootloader_ver = can_data9[6];
                                status9 =  false;
                                canTransmit(0x1806F4F6, 4U, can_ack);
                                //collect can data

                                //send OK to CAN 
                                
                                goto label4;



                                break;
                            case 0xF2:


                                firmware_loc = (uint32_t) can_data9[4] << 24 | (uint32_t) can_data9[3] << 16 | can_data9[2] << 8 | can_data9[1];
                                sprintf(bootloader_string, "BMSINFO%lu,%u,%u,%luBMSINFO\r\n", BMS_ID, firmware_ver, bootloader_ver, firmware_loc);
                                canTransmit(0x1806F4F6, 4U, can_ack);
                                uart_transmit(bootloader_string);
                                //collect can data
                                //send OK to CAN 
                                // send to GUI
                                waitForNextGUIMsg();
                                command_service();

                                if (OBD_event == BMS_bootloader_event) {

                                    status9 =  false;
                                    goto label3;
                                } else {
                                    uart_transmit(AT_error);
                                }



                                break;
                            default:
                                uart_transmit(timeout_msg);
                                canTransmit(0x1806F4F6, 8U, can_timeOut);

                                break;
                        }

                        break;

                    case '3':
                        //recieve 64 bytes frm GUI and send it to CAN
                        
                        for(i= 0 ;i<200 ;i++){
                            
                            BMS_bootloader_string_array[i] = NULL;
                            
                        }
                     
                               j=0;
                             k =0;
                             count = 0;
                            char* token1 = strtok(command, ",");
                            char *ptr1;
                            while (token1 != NULL) {

                                BMS_bootloader_string_array[k] = strtol(token1, &ptr1, 10); //CONVERT STRING TO INTEGER BASE 10
                                k++;

                                token1 = strtok(NULL, ",");

                            }
                            for(i=2 ; BMS_bootloader_string_array[i] != NULL ; i++)
                            {
                                count++;
                            }
                            if(count == BMS_bootloader_string_array[1]){
                                
                               BMS_bootloader_packet_num = 0x01;
                               BMS_bootloader_data[0] =  BMS_bootloader_packet_num;
                               BMS_bootloader_data[1] = BMS_bootloader_string_array[1];
                               for(i=2 ; i<8 ; i++ ){
                                   
                                    BMS_bootloader_data[j + 2] = BMS_bootloader_string_array[i];
                                    j++;
                               }
                                canTransmit(0x1806F4F5, 8U, BMS_bootloader_data);
                                waitForNextCANMsg();
                                if ((status8 != true) || (can_data8[0] != 0x01)) {
                                        uart_transmit(timeout_msg);
                                        canTransmit(0x1806F4F6, 8U, can_timeOut);
                                        
                                        return;
                                 }
                                status8 =  false;
                                BMS_bootloader_packet_num++;
                                int frame_count;
                                frame_count = ceil(( count - 6.00)/8.00);
                                
                                
                                     for (k = 0; k < frame_count; k++) {

                                    BMS_bootloader_data[0] = BMS_bootloader_packet_num;
                                    for (j = 1; j < 8; j++) {

                                        BMS_bootloader_data[j] = BMS_bootloader_string_array[i];
                                        i++;


                                    }
                                    canTransmit(0x1806F4F5, 8U, BMS_bootloader_data);
                                    waitForNextCANMsg();
                                    if ((status8 != true) || (can_data8[0] != 0x01)) {
                                        uart_transmit(timeout_msg);
                                        canTransmit(0x1806F4F6, 8U, can_timeOut);
                                        
                                        return;
                                    }
                                     status8 =  false;
                                    BMS_bootloader_packet_num++;



                               }
                               
                                uart_transmit(gui_ack);
                               
                                
                                
                            
                            }else{
                                 uart_transmit(AT_error);
                            }
                            
                          
                            waitForNextGUIMsg();
                            OBD_event = default_event;
                            command_service();

                            if (OBD_event == BMS_bootloader_event) {


                                goto label3;
                            }


                        

                        break;

                    case '4':
                        
                        canTransmit(0x1806F4F6, 4U, can_done);
                        uart_transmit(gui_ack);

                        break;

                    default:

                        uart_transmit(AT_error);
                        break;

                }
                status9 = false;
                can_rx_receive_flag = false;

            } else {
                uart_transmit(AT_error);
            }








    }
}

