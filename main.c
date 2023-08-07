#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/uart3.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/tmr2.h"
#include  "pic18f57q84.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>



extern uint8_t rx_buffer[150], baud_eeprom, can_baud_eeprom;
extern volatile uint8_t rx_buffer_index;
extern volatile bool rx_receive_complete;
extern char command[150];

extern uint8_t can_data1[8], can_data2[8], can_data3[8], can_data4[8], can_data5[8], can_data6[8], can_data7[8], can_data8[8], can_data9[8], can_data10[8];
extern volatile uint32_t id;
extern bool can_rx_receive_flag = false, status1, status2, status3, status4, status5, status6, status7, status8, status9, status10;
long int start_point = 0, end_point = 0;
int m = 0, k = 0, i = 0, count = 0, j;
uint16_t temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, cell_voltage[20];
uint8_t err1, err2, err3, pack_voltage, pack_current, SOC;
char AT_command[8];
bool handshake = false, usb_ready = false, timeout = false;

CAN_MSG_OBJ TRANSMIT_MESSAGE;

uint16_t config_data[16], can_rx_conf[15], can_rx_sd[64];
uint8_t sd_data[8], can_rx_sd_byte;
char transmit1[150], transmit2[150], transmit3[150], transmit4[80], transmit5[300], timeout_msg[] = "AT+TIME\r\n", gui_ack[10] = "AT+DONE\r\n", AT_error[] = "AT+ERRR\r\n";
char test[] = {"AT+TEST\r\n"};

uint8_t can_data_req[4] = {0x04, 0x0D, 0x0F, 0x63}, can_tx[8], can_tx_conf[22];
uint8_t Tx_baud_500[4] = {0x04, 0x0D, 0x0F, 0x64};
uint8_t Tx_baud_250[4] = {0x04, 0x0D, 0x0F, 0x65};
uint8_t Tx_baud_125[4] = {0x04, 0x0D, 0x0F, 0x66};
uint8_t can_sft_rst[4] = {0x00, 0xFF, 0x01, 0x00};
uint8_t can_config_req[4] = {0x00, 0x01, 0x00, 0x00};
uint8_t can_ack[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t can_timeOut[8] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void TMR2_DefaultInterruptHandler(void) {

    timeout = true;

}

void comns_test();
void AT_command_search();
void obd_state_machine();
void canTransmit(uint32_t messageID, uint8_t data_length, uint8_t *TransmitData);
void waitForNextMsg();
void uart_transmit(char *uart_data);
void B2G_config_data_flow();
void sd_data_flow();

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
    waitForNextMsg();
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
    waitForNextMsg();
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
    waitForNextMsg();
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
    waitForNextMsg();
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

void sd_data_flow() {


    //flow control
    waitForNextMsg();
    if (status10 != true) {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
        return;
    }

label1:
    switch (can_data10[0]) {
        case 0x10: //6 bytes
            j = 0;
            for (i = 1; i < 8; i++) {
                if (i == 1) {
                    can_rx_sd_byte = can_data10[i];
                } else {
                    can_rx_sd[j] = can_data10[i];
                    j++;
                }
            }

            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x20: //6+7 bytes
            j = 6;
            for (i = 1; i < 8; i++) {
                can_rx_sd[j] = can_data10[i];
                j++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x21: //6+7+7 bytes
            j = 13;
            for (i = 1; i < 8; i++) {
                can_rx_sd[j] = can_data10[i];
                j++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x22: //6+7+7+7 bytes
            j = 20;
            for (i = 1; i < 8; i++) {
                can_rx_sd[j] = can_data10[i];
                j++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x23://6+7+7+7+7 bytes
            j = 27;
            for (i = 1; i < 8; i++) {
                can_rx_sd[j] = can_data10[i];
                j++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x24://6+7+7+7+7+7 bytes
            j = 34;
            for (i = 1; i < 8; i++) {
                can_rx_sd[j] = can_data10[i];
                j++;
            }
            canTransmit(0x1806F4F6, 4U, can_ack);
            status10 = false;
            can_rx_receive_flag = false;
            break;

        case 0x25://6+7+7+7+7+7+7 = 48 bytes(1 packet)
            j = 41;
            for (i = 1; i < 8; i++) {
                can_rx_sd[j] = can_data10[i];
                j++;
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
    waitForNextMsg();
    if (status10 == true) {
        goto label1; //next frame
    } else if ((status8 == true) && (can_data8[0] == 0x01)) {


        sprintf(transmit5, "SD%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uSD\r\n", can_rx_sd_byte, can_rx_sd[0], can_rx_sd[1], can_rx_sd[2], can_rx_sd[3], can_rx_sd[4], can_rx_sd[5], can_rx_sd[6], can_rx_sd[7], can_rx_sd[8], can_rx_sd[9], can_rx_sd[10], can_rx_sd[11], can_rx_sd[12], can_rx_sd[13], can_rx_sd[14], can_rx_sd[15], can_rx_sd[16], can_rx_sd[17], can_rx_sd[18], can_rx_sd[19], can_rx_sd[20], can_rx_sd[21], can_rx_sd[22], can_rx_sd[23], can_rx_sd[24], can_rx_sd[25], can_rx_sd[26], can_rx_sd[27], can_rx_sd[28], can_rx_sd[29], can_rx_sd[30], can_rx_sd[31], can_rx_sd[32], can_rx_sd[33], can_rx_sd[34], can_rx_sd[35], can_rx_sd[36], can_rx_sd[37], can_rx_sd[38], can_rx_sd[39], can_rx_sd[40], can_rx_sd[41], can_rx_sd[42], can_rx_sd[43], can_rx_sd[44], can_rx_sd[45], can_rx_sd[46], can_rx_sd[47]);
        uart_transmit(transmit5);
        /********transmit packet to gui*************/

        // memset(can_rx_sd , 0 , sizeof(can_rx_sd));
        for (i = 0; i < 64; i++) {
            can_rx_sd[i] = 0;
        }
        canTransmit(0x1806F4F6, 4U, can_ack);
        waitForNextMsg();

        if (status10 == true) {
            goto label1; //next packet
        } else {
            uart_transmit(timeout_msg);
            canTransmit(0x1806F4F6, 8U, can_timeOut);
        }

    } else if ((status8 == true) && (can_data8[1] = 0x02)) {

        sprintf(transmit5, "SD%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uSD\r\n", can_rx_sd_byte, can_rx_sd[0], can_rx_sd[1], can_rx_sd[2], can_rx_sd[3], can_rx_sd[4], can_rx_sd[5], can_rx_sd[6], can_rx_sd[7], can_rx_sd[8], can_rx_sd[9], can_rx_sd[10], can_rx_sd[11], can_rx_sd[12], can_rx_sd[13], can_rx_sd[14], can_rx_sd[15], can_rx_sd[16], can_rx_sd[17], can_rx_sd[18], can_rx_sd[19], can_rx_sd[20], can_rx_sd[21], can_rx_sd[22], can_rx_sd[23], can_rx_sd[24], can_rx_sd[25], can_rx_sd[26], can_rx_sd[27], can_rx_sd[28], can_rx_sd[29], can_rx_sd[30], can_rx_sd[31], can_rx_sd[32], can_rx_sd[33], can_rx_sd[34], can_rx_sd[35], can_rx_sd[36], can_rx_sd[37], can_rx_sd[38], can_rx_sd[39], can_rx_sd[40], can_rx_sd[41], can_rx_sd[42], can_rx_sd[43], can_rx_sd[44], can_rx_sd[45], can_rx_sd[46], can_rx_sd[47]);
        uart_transmit(transmit5);
        uart_transmit(gui_ack);
        /********transmit packet to gui*************/
    } else {
        uart_transmit(timeout_msg);
        canTransmit(0x1806F4F6, 8U, can_timeOut);
    }





}

void comns_test() {
    while (handshake == false) {
        //Low();
        uart_transmit(test);
        if (rx_receive_complete == true) {
            for (i = 0; i < 150; i++) {

                rx_buffer[i] = 0;
            }

            rx_buffer_index = 0;

            AT_command_search();


            if (!(strncmp(AT_command, "AT+TEST=", 8))) {
                count = 0;

                for (i = start_point; i < end_point; i++) {
                    count++;
                }
                if (count == 10) {


                    if ((command[start_point + 8] == 'O') & (command[start_point + 9] == 'K')) {
                        usb_ready = true;
                        handshake = true;
                    }


                }
            }
            rx_receive_complete = false;


        }
        for (i = 0; i < 150; i++) {
            command[i] = '\0';
        }
        for (i = 0; i < 8; i++) {
            AT_command[i] = '\0';
        }


    }
}

void AT_command_search() {
    m = 0;
    char* pos0 = strstr(command, "AT+TEST=");
    char* pos1 = strstr(command, "AT+DATA=");
    char* pos2 = strstr(command, "AT+BAUD=");
    char* pos3 = strstr(command, "AT+CANB=");
    char* pos4 = strstr(command, "AT+CONF=");
    char* pos5 = strstr(command, "\r\n");
    char* pos6 = strstr(command, "AT+REST=");
    if (pos0 != NULL) {
        if (pos5 != NULL) {
            end_point = pos5 - command;

            start_point = (pos0 - command);
            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }

    } else if (pos1 != NULL) {
        if (pos5 != NULL) {

            end_point = (pos5 - command);
            start_point = (pos1 - command);
            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }
    } else if (pos2 != NULL) {
        if (pos5 != NULL) {

            end_point = (pos5 - command);



            start_point = (pos2 - command);

            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }

    } else if (pos3 != NULL) {
        if (pos5 != NULL) {

            end_point = (pos5 - command);



            start_point = (pos3 - command);

            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }

    } else if (pos4 != NULL) {
        if (pos5 != NULL) {

            end_point = (pos5 - command);

            start_point = (pos4 - command);

            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }

    } else if (pos6 != NULL) {
        if (pos5 != NULL) {

            end_point = (pos5 - command);

            start_point = (pos6 - command);

            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }

    } else {
        uart_transmit(AT_error);
    }

}

void canTransmit(uint32_t messageID, uint8_t data_length, uint8_t *TransmitData) {
    TRANSMIT_MESSAGE.msgId = messageID;
    TRANSMIT_MESSAGE.field.brs = CAN_NON_BRS_MODE;
    TRANSMIT_MESSAGE.field.dlc = data_length;
    TRANSMIT_MESSAGE.field.formatType = CAN_2_0_FORMAT;
    TRANSMIT_MESSAGE.field.frameType = CAN_FRAME_DATA;
    TRANSMIT_MESSAGE.field.idType = CAN_FRAME_EXT;
    TRANSMIT_MESSAGE.data = TransmitData;

    if (CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(TXQ) & CAN_TX_FIFO_AVAILABLE)) {
        CAN1_Transmit(TXQ, &TRANSMIT_MESSAGE);
    }
}

void waitForNextMsg() {
    can_rx_receive_flag = false;
    timeout = false;
    TMR2_Start();
    do {

        if (timeout == true) {
            uart_transmit(timeout_msg);
            canTransmit(0x1806F4F6, 8U, can_timeOut);
            timeout = false;
            break;

        }

    } while (can_rx_receive_flag != true);
    TMR2_Stop();


}

void uart_transmit(char *uart_data) {

    for (i = 0; i < strlen(uart_data); i++) {
        UART2_Write(uart_data[i]);
    }
}

void main(void) {



    // baud_eeprom = DATAEE_ReadByte(0x00);
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    UART3_SetRxInterruptHandler(UART3_Receive_ISR);
    for (i = 0; i < 20; i++) {
        cell_voltage[i] = 0;
    }


    temperature1 = 0;
    temperature2 = 0;
    temperature3 = 0;
    temperature4 = 0;
    temperature5 = 0;
    temperature6 = 0;


    pack_voltage = 0;
    pack_current = 0;
    SOC = 0;
    err1 = 0;
    err2 = 0;
    err3 = 0;



    status1 = false;
    status2 = false;
    status3 = false;
    status4 = false;
    status5 = false;
    status6 = false;
    status7 = false;
    status8 = false;
    status9 = false;
    status10 = false;





    rx_receive_complete = false;


    for (i = 0; i < 150; i++) {
        command[i] = '\0';
    }
    for (i = 0; i < 8; i++) {
        AT_command[i] = '\0';
    }



    __delay_ms(5000);






    while (1) {

        obd_state_machine();

    }


    // Add your application code
}

void obd_state_machine() {

    comns_test();

    if (rx_receive_complete == true) {

        // IO_RB0_SetLow();
        for (i = 0; i < 150; i++) {
            rx_buffer[i] = 0;
        }


        rx_buffer_index = 0;
        AT_command_search();


        if (!(strncmp(AT_command, "AT+BAUD=", 8))) //SERIAL BAUD RATE
        {
            count = 0;

            for (i = start_point; i < end_point; i++) {
                count++;
            }
            if (count == 9) {

                switch (command[start_point + 8]) {
                    case '1': //9600
                        U2BRGL = 0x03;
                        U2BRGH = 0x01;
                        U3BRGL = 0x03;
                        U3BRGH = 0x01;

                        handshake = false;
                        break;
                    case '2': //57600
                        U2BRGL = 0x2A;
                        U2BRGH = 0x00;
                        U3BRGL = 0x2A;
                        U3BRGH = 0x00;
                        handshake = false;
                        break;
                    case '3': //115200
                        U2BRGL = 0x15;
                        U2BRGH = 0x00;
                        U3BRGL = 0x15;
                        U3BRGH = 0x00;
                        //DATAEE_WriteByte(0x00, 0x03);
                        handshake = false;
                        break;
                    default:
                        uart_transmit(AT_error);
                        break;


                }

            } else {
                uart_transmit(AT_error);

            }


        }
        if (!(strncmp(AT_command, "AT+CANB=", 8))) //CAN BAUD RATE
        {
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

        }

        if (!(strncmp(AT_command, "AT+DATA=", 8))) {
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


        }

        if (!(strncmp(AT_command, "AT+REST=", 8))) //SOFTWARE RESET
        {
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


                waitForNextMsg();

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
        }

        if (!(strncmp(AT_command, "AT+CONF=", 8))) //CONFIG DATA
        {
            can_rx_receive_flag = false;



            uint8_t msb, lsb, j = 0;
            count = 0;
            k = 0;


            for (i = start_point; i < end_point; i++) {
                count++;

            }

            switch (count) {
                case 68:
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
                    /******************************config file BMS to gui***************************************************/
                case 9:
                    if (command[start_point + 8] == '2') {



                        canTransmit(0x1806F4F5, 4U, can_config_req);
                        waitForNextMsg();

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
                            waitForNextMsg();
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

                                sprintf(transmit4, "CON%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%uCON\r\n", can_rx_conf[0], can_rx_conf[1], can_rx_conf[2], can_rx_conf[3], can_rx_conf[4], can_rx_conf[5], can_rx_conf[6], can_rx_conf[7], can_rx_conf[8], can_rx_conf[9], can_rx_conf[10], can_rx_conf[11], can_rx_conf[12], can_rx_conf[13], can_rx_conf[14]);
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

                default:uart_transmit(AT_error);
            }
        }

        for (i = 0; i < 150; i++) {
            command[i] = '\0';
        }
        for (i = 0; i < 8; i++) {
            AT_command[i] = '\0';
        }


        rx_receive_complete = false;
    } else {

        canTransmit(0x1806F4F4, 4U, can_data_req);
        if (status1)//0x0100FFF4
        {
            err1 = can_data1[0];
            SOC = can_data1[1];
            pack_current = (can_data1[3] << 8) | can_data1[2];
            pack_voltage = (can_data1[5] << 8) | can_data1[4];
            err2 = can_data1[6];
            err3 = can_data1[7];
            status1 = false;

        }
        if (status2) //0x0101FFF4
        {

            cell_voltage[0] = (can_data2[1] << 8) | can_data2[0];
            cell_voltage[1] = (can_data2[3] << 8) | can_data2[2];
            cell_voltage[2] = (can_data2[5] << 8) | can_data2[4];
            cell_voltage[3] = (can_data2[7] << 8) | can_data2[6];
            status2 = false;
        }

        if (status3)//0x0102FFF4
        {

            cell_voltage[4] = (can_data3[1] << 8) | can_data3[0];
            cell_voltage[5] = (can_data3[3] << 8) | can_data3[2];
            cell_voltage[6] = (can_data3[5] << 8) | can_data3[4];
            cell_voltage[7] = (can_data3[7] << 8) | can_data3[6];
            status3 = false;

        }

        if (status4)//0x0103FFF4
        {
            cell_voltage[8] = (can_data4[1] << 8) | can_data4[0];
            cell_voltage[9] = (can_data4[3] << 8) | can_data4[2];
            cell_voltage[10] = (can_data4[5] << 8) | can_data4[4];
            cell_voltage[11] = (can_data4[7] << 8) | can_data4[6];
            status4 = false;
        }

        if (status5)//0x0104FFF4
        {
            cell_voltage[12] = (can_data5[1] << 8) | can_data5[0];
            cell_voltage[13] = (can_data5[3] << 8) | can_data5[2];
            cell_voltage[14] = (can_data5[5] << 8) | can_data5[4];
            cell_voltage[15] = (can_data5[7] << 8) | can_data5[6];
            status5 = false;

        }
        if (status6) //0x0105FFF4
        {
            cell_voltage[16] = (can_data6[1] << 8) | can_data6[0];
            cell_voltage[17] = (can_data6[3] << 8) | can_data6[2];
            cell_voltage[18] = (can_data6[5] << 8) | can_data6[4];
            cell_voltage[19] = (can_data6[7] << 8) | can_data6[6];
            status6 = false;

        }


        if (status7)//0x0111FFF4
        {
            temperature1 = can_data7[0];
            temperature2 = can_data7[1];
            temperature3 = can_data7[2];
            temperature4 = can_data7[3];
            temperature5 = can_data7[4];
            temperature6 = can_data7[5];
            status7 = false;

        }

        //            temperature1 = 0x1A;
        //            temperature2 = 0x1E;
        //            temperature3 = 0x1C;
        //            temperature4 = 0x23;
        //            temperature5 = 0x1C;
        //            temperature6 = 0x19;


        sprintf(transmit1, "VCS%u,%u,%u,%u,%u,%uVCS\n\r", err1, pack_voltage, pack_current, SOC, err2, err3);
        sprintf(transmit2, "#%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u*\n\r", cell_voltage[0], cell_voltage[1], cell_voltage[2], cell_voltage[3], cell_voltage[4], cell_voltage[5], cell_voltage[6], cell_voltage[7], cell_voltage[8], cell_voltage[9], cell_voltage[10], cell_voltage[11], cell_voltage[12], cell_voltage[13], cell_voltage[14], cell_voltage[15], cell_voltage[16], cell_voltage[17], cell_voltage[18], cell_voltage[19]);
        sprintf(transmit3, "^%u,%u,%u,%u,%u,%uC\n\r", temperature1, temperature2, temperature3, temperature4, temperature5, temperature6);


        //        for(i=0 ; i<20; i++)
        //        {
        //            cell_voltage[i] = cell_voltage[i] + 10;
        //            
        //            if(cell_voltage[i] == 4300)
        //            {
        //                cell_voltage[i] = 3100;
        //            }
        //        }





        if (usb_ready == true) {
            //IO_RB0_SetHigh();
            uart_transmit(transmit1);
            uart_transmit(transmit2);
            uart_transmit(transmit3);
        }





    }
}
