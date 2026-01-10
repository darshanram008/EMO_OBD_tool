#include "OBD_tool_events.h"
#include "flow_control.h"
#include "BMS_data_handle.h"


uint8_t can_ack[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t sd_data[8], can_rx_sd_byte;
char timeout_msg[] = "AT+TIME\r\n", gui_ack[10] = "AT+DONE\r\n", AT_error[] = "AT+ERRR\r\n";
uint8_t  can_tx_conf[22], can_tx[8];
uint8_t can_sft_rst[4] = {0x00, 0xFF, 0x01, 0x00};
uint8_t can_config_req[4] = {0x00, 0x01, 0x00, 0x00};
uint8_t can_timeOut[8] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
bool timeout2 = false, timeout0= false;
 char transmit1[150], transmit2[150], transmit3[150], transmit4[80], transmit5[300]; 
void TMR2_DefaultInterruptHandler(void) {

    timeout2 = true;

}
void TMR0_DefaultInterruptHandler(void){
    
    timeout0 = true;
}

void B2G_config_data_flow() {


    /********pack and send first data********/
    can_tx[0] = 0x01;
    can_tx[1] = can_tx_conf[0];
    can_tx[2] = can_tx_conf[1];
    can_tx[3] = can_tx_conf[2];
    can_tx[4] = can_tx_conf[3];
    can_tx[5] = can_tx_conf[4];
    can_tx[6] = can_tx_conf[5];
    can_tx[7] = can_tx_conf[14];
    canTransmit(0x1806F4F5, 8U, can_tx);
    /*******wait for ok from BMS*/
    waitForNextCANMsg();
    /********************************/
    if ((status8 != true) || (can_data8[0] != 0x01)) {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
        return;
    }

    /**********pack and send second data***/
    can_tx[0] = 0x02;
    can_tx[1] = can_tx_conf[6];
    can_tx[2] = can_tx_conf[7];
    can_tx[3] = can_tx_conf[8];
    can_tx[4] = can_tx_conf[9];
    can_tx[5] = can_tx_conf[10];
    can_tx[6] = can_tx_conf[11];
    can_tx[7] = can_tx_conf[15];
    canTransmit(0x1806F4F5, 8U, can_tx);
    status8 = false;
    can_rx_receive_flag = false;
    /*****wait for OK *******/
    waitForNextCANMsg();
    /***************************/
    if ((status8 != true) || (can_data8[0] != 0x01)) {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
        return;
    }
    /**********pack and send third data***/
    can_tx[0] = 0x03;
    can_tx[1] = can_tx_conf[12];
    can_tx[2] = can_tx_conf[13];
    can_tx[3] = can_tx_conf[16];
    can_tx[4] = can_tx_conf[17];
    can_tx[5] = can_tx_conf[18];
    can_tx[6] = can_tx_conf[19];
    can_tx[7] = can_tx_conf[20];
    canTransmit(0x1806F4F5, 8U, can_tx);
    status8 = false;
    can_rx_receive_flag = false;
    /******wait for OK ********/
    waitForNextCANMsg();
    /********************************/
    if ((status8 != true) || (can_data8[0] != 0x01)) {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
        return;
    }
    /*****pack and send fourth data***/
    can_tx[0] = 0x04;
    can_tx[1] = can_tx_conf[21];
    can_tx[2] = 0x00;
    can_tx[3] = 0x00;
    can_tx[4] = 0x00;
    can_tx[5] = 0x00;
    can_tx[6] = 0x00;
    can_tx[7] = 0x00;
    canTransmit(0x1806F4F5, 8U, can_tx);
    status8 = false;
    can_rx_receive_flag = false;
    /******wait for ok ********/
    waitForNextCANMsg();
    /********************************/
    if ((status8 != true) || (can_data8[0] != 0x01)) {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
        return;
    }

    /********All params sent, acknwoledge GUI****/
    uart_transmit(gui_ack);
    status8 = false;
    can_rx_receive_flag = false;


}


void waitForNextCANMsg() {
    can_rx_receive_flag = false;
    timeout2 = false;
    TMR2_Start();
    do {

        if (timeout2 == true) {
            uart_transmit(timeout_msg);
            canTransmit(0x1806F4F6, 8U, can_timeOut);
            timeout2 = false;
            break;

        }

    } while (can_rx_receive_flag != true);
    TMR2_Stop();


}
void waitForNextGUIMsg(){
    rx_receive_complete = false;
    timeout0 = false;
    TMR0_StartTimer();
    do{
        
        if (timeout0 == true) {
            uart_transmit(timeout_msg);
            canTransmit(0x1806F4F6, 8U, can_timeOut);
            timeout0 = false;
            break;

        }
        
        
        
    }while(rx_receive_complete != true);
     TMR0_StopTimer();
    
}



void sd_data_flow() {

    //flow control
    uint8_t  can_rx_sd_index, i;
    waitForNextCANMsg();
    if (status10 != true) {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
        return;
    }

label1:
    switch (can_data10[0]) {
        case 0x10: //6 bytes
            can_rx_sd_index = 0;
            for (i = 1; i < 8; i++) {
                if (i == 1) {
                    can_rx_sd_byte = can_data10[i];
                } else {
                    can_rx_sd[can_rx_sd_index] = can_data10[i];
                    can_rx_sd_index++;
                }
            }

            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x20: //6+7 bytes
            can_rx_sd_index = 6;
            for (i = 1; i < 8; i++) {
                can_rx_sd[can_rx_sd_index] = can_data10[i];
                can_rx_sd_index++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x21: //6+7+7 bytes
            can_rx_sd_index = 13;
            for (i = 1; i < 8; i++) {
                can_rx_sd[can_rx_sd_index] = can_data10[i];
                can_rx_sd_index++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x22: //6+7+7+7 bytes
            can_rx_sd_index = 20;
            for (i = 1; i < 8; i++) {
                can_rx_sd[can_rx_sd_index] = can_data10[i];
                can_rx_sd_index++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x23://6+7+7+7+7 bytes
            can_rx_sd_index = 27;
            for (i = 1; i < 8; i++) {
                can_rx_sd[can_rx_sd_index] = can_data10[i];
                can_rx_sd_index++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x24://6+7+7+7+7+7 bytes
            can_rx_sd_index = 34;
            for (i = 1; i < 8; i++) {
                can_rx_sd[can_rx_sd_index] = can_data10[i];
                can_rx_sd_index++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x25://6+7+7+7+7+7+7 = 48 bytes(1 packet)
            can_rx_sd_index = 41;
            for (i = 1; i < 8; i++) {
                can_rx_sd[can_rx_sd_index] = can_data10[i];
                can_rx_sd_index++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        default:
            status10 = false;
            can_rx_receive_flag = false;
            break;

    }
    waitForNextCANMsg();
    if (status10 == true) {
        goto label1; //next frame
    } else if ((status8 == true) && (can_data8[0] == 0x01)) {


        sprintf(transmit5, "SD_CARD%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uSD_CARD\r\n", can_rx_sd_byte, can_rx_sd[0], can_rx_sd[1], can_rx_sd[2], can_rx_sd[3], can_rx_sd[4], can_rx_sd[5], can_rx_sd[6], can_rx_sd[7], can_rx_sd[8], can_rx_sd[9], can_rx_sd[10], can_rx_sd[11], can_rx_sd[12], can_rx_sd[13], can_rx_sd[14], can_rx_sd[15], can_rx_sd[16], can_rx_sd[17], can_rx_sd[18], can_rx_sd[19], can_rx_sd[20], can_rx_sd[21], can_rx_sd[22], can_rx_sd[23], can_rx_sd[24], can_rx_sd[25], can_rx_sd[26], can_rx_sd[27], can_rx_sd[28], can_rx_sd[29], can_rx_sd[30], can_rx_sd[31], can_rx_sd[32], can_rx_sd[33], can_rx_sd[34], can_rx_sd[35], can_rx_sd[36], can_rx_sd[37], can_rx_sd[38], can_rx_sd[39], can_rx_sd[40], can_rx_sd[41], can_rx_sd[42], can_rx_sd[43], can_rx_sd[44], can_rx_sd[45], can_rx_sd[46], can_rx_sd[47]);
        uart_transmit(transmit5);
        /********transmit packet to gui*************/

        // memset(can_rx_sd , 0 , sizeof(can_rx_sd));
        for (i = 0; i < 64; i++) {
            can_rx_sd[i] = 0;
        }
        canTransmit(0x1806F4F6, 4U, can_ack);
        waitForNextCANMsg();

        if (status10 == true) {
            goto label1; //next packet
        } else {
            uart_transmit(timeout_msg);
            canTransmit(0x1806F4F6, 8U, can_timeOut);
        }

    } else if ((status8 == true) && (can_data8[0] = 0x02)) {

        sprintf(transmit5, "SD_CARD%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uSD_CARD\r\n", can_rx_sd_byte, can_rx_sd[0], can_rx_sd[1], can_rx_sd[2], can_rx_sd[3], can_rx_sd[4], can_rx_sd[5], can_rx_sd[6], can_rx_sd[7], can_rx_sd[8], can_rx_sd[9], can_rx_sd[10], can_rx_sd[11], can_rx_sd[12], can_rx_sd[13], can_rx_sd[14], can_rx_sd[15], can_rx_sd[16], can_rx_sd[17], can_rx_sd[18], can_rx_sd[19], can_rx_sd[20], can_rx_sd[21], can_rx_sd[22], can_rx_sd[23], can_rx_sd[24], can_rx_sd[25], can_rx_sd[26], can_rx_sd[27], can_rx_sd[28], can_rx_sd[29], can_rx_sd[30], can_rx_sd[31], can_rx_sd[32], can_rx_sd[33], can_rx_sd[34], can_rx_sd[35], can_rx_sd[36], can_rx_sd[37], can_rx_sd[38], can_rx_sd[39], can_rx_sd[40], can_rx_sd[41], can_rx_sd[42], can_rx_sd[43], can_rx_sd[44], can_rx_sd[45], can_rx_sd[46], can_rx_sd[47]);
        uart_transmit(transmit5);
        uart_transmit(gui_ack);
        /********transmit packet to gui*************/
    } else {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
    }





}