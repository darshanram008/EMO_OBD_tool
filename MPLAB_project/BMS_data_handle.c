#include "BMS_data_handle.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/can1.h"
uint8_t can_baud_eeprom;

volatile uint32_t id;
bool can_rx_receive_flag = false, status1, status2, status3, status4, status5, status6, status7, status8, status9, status10;
uint16_t temperature1, temperature2, temperature3, temperature4, temperature5, temperature6, cell_voltage[20];
uint16_t can_data1[8], can_data2[8], can_data3[8], can_data4[8], can_data5[8], can_data6[8], can_data7[8], can_data8[8], can_data9[8], can_data10[8];
uint16_t err1, err2, err3, pack_voltage, pack_current, SOC;
 void CAN1_BitRateConfiguration(void) {
    can_baud_eeprom = DATAEE_ReadByte(0x00);
    if (can_baud_eeprom == 0x01) {
        C1NBTCFGL = 0x13;
        C1NBTCFGH = 0x13;
        C1NBTCFGU = 0x3A; //125kbps
        C1NBTCFGT = 0x00;
        //can_baud_125 = false;
    }
    if ((can_baud_eeprom == 0xff) | (can_baud_eeprom == 0x02)) {
        C1NBTCFGL = 0x09;
        C1NBTCFGH = 0x09; /////250kbps
        C1NBTCFGU = 0x1c;
        C1NBTCFGT = 0x00;
        //can_baud_250 = false;


    }
    if (can_baud_eeprom == 0x03) {
        C1NBTCFGL = 0x04;
        C1NBTCFGH = 0x04;
        C1NBTCFGU = 0x0D; ////500kbps
        C1NBTCFGT = 0x00;
        //can_baud_500 = false;

    }

}

void canTransmit(uint32_t messageID, uint8_t data_length, uint8_t *TransmitData) {
    CAN_MSG_OBJ TRANSMIT_MESSAGE;
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


void CAN_main_data(){
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
            
}




 void DefaultFIFO1NotEmptyHandler(void) {
    TU16ACON0bits.ON = 0; //stop the periodic heartbeat, go into response mode
    CAN_MSG_OBJ Receive_message;


    while (1) {
        if (CAN1_ReceivedMessageCountGet() > 0) {
            if (true == CAN1_ReceiveFrom(FIFO1, &Receive_message)) //receive the message
            {
                break;
            }
        }
    }
    id = Receive_message.msgId;
    can_rx_receive_flag = true;


    if (id == 0x0100FFF4) {
        can_data1[0] = Receive_message.data[0];
        can_data1[1] = Receive_message.data[1];
        can_data1[2] = Receive_message.data[2];
        can_data1[3] = Receive_message.data[3];
        can_data1[4] = Receive_message.data[4];
        can_data1[5] = Receive_message.data[5];
        can_data1[6] = Receive_message.data[6];
        can_data1[7] = Receive_message.data[7];
        status1 = true;

    }
    if (id == 0x0101FFF4) {
        can_data2[0] = Receive_message.data[0];
        can_data2[1] = Receive_message.data[1];
        can_data2[2] = Receive_message.data[2];
        can_data2[3] = Receive_message.data[3];
        can_data2[4] = Receive_message.data[4];
        can_data2[5] = Receive_message.data[5];
        can_data2[6] = Receive_message.data[6];
        can_data2[7] = Receive_message.data[7];
        status2 = true;


    }
    if (id == 0x0102FFF4) {
        can_data3[0] = Receive_message.data[0];
        can_data3[1] = Receive_message.data[1];
        can_data3[2] = Receive_message.data[2];
        can_data3[3] = Receive_message.data[3];
        can_data3[4] = Receive_message.data[4];
        can_data3[5] = Receive_message.data[5];
        can_data3[6] = Receive_message.data[6];
        can_data3[7] = Receive_message.data[7];
        status3 = true;
    }
    if (id == 0x0103FFF4) {
        can_data4[0] = Receive_message.data[0];
        can_data4[1] = Receive_message.data[1];
        can_data4[2] = Receive_message.data[2];
        can_data4[3] = Receive_message.data[3];
        can_data4[4] = Receive_message.data[4];
        can_data4[5] = Receive_message.data[5];
        can_data4[6] = Receive_message.data[6];
        can_data4[7] = Receive_message.data[7];
        status4 = true;
    }
    if (id == 0x0104FFF4) {
        can_data5[0] = Receive_message.data[0];
        can_data5[1] = Receive_message.data[1];
        can_data5[2] = Receive_message.data[2];
        can_data5[3] = Receive_message.data[3];
        can_data5[4] = Receive_message.data[4];
        can_data5[5] = Receive_message.data[5];
        can_data5[6] = Receive_message.data[6];
        can_data5[7] = Receive_message.data[7];
        status5 = true;

    }


    if (id == 0x0105FFF4) {
        can_data6[0] = Receive_message.data[0];
        can_data6[1] = Receive_message.data[1];
        can_data6[2] = Receive_message.data[2];
        can_data6[3] = Receive_message.data[3];
        can_data6[4] = Receive_message.data[4];
        can_data6[5] = Receive_message.data[5];
        can_data6[6] = Receive_message.data[6];
        can_data6[7] = Receive_message.data[7];
        status6 = true;

    }
    if (id == 0x0111FFF4) {
        can_data7[0] = Receive_message.data[0];
        can_data7[1] = Receive_message.data[1];
        can_data7[2] = Receive_message.data[2];
        can_data7[3] = Receive_message.data[3];
        can_data7[4] = Receive_message.data[4];
        can_data7[5] = Receive_message.data[5];
        can_data7[6] = Receive_message.data[6];
        can_data7[7] = Receive_message.data[7];
        status7 = true;
    }

    if (id == 0x0110FFF4) {
        can_data8[0] = Receive_message.data[0];
        can_data8[1] = Receive_message.data[1];
        can_data8[2] = Receive_message.data[2];
        can_data8[3] = Receive_message.data[3];
        can_data8[4] = Receive_message.data[4];
        can_data8[5] = Receive_message.data[5];
        can_data8[6] = Receive_message.data[6];
        can_data8[7] = Receive_message.data[7];
        status8 = true;

    }
    if (id == 0x0121FFF5) {
        can_data9[0] = Receive_message.data[0];
        can_data9[1] = Receive_message.data[1];
        can_data9[2] = Receive_message.data[2];
        can_data9[3] = Receive_message.data[3];
        can_data9[4] = Receive_message.data[4];
        can_data9[5] = Receive_message.data[5];
        can_data9[6] = Receive_message.data[6];
        can_data9[7] = Receive_message.data[7];
        status9 = true;

    }
    if (id == 0x0121FFF4) {
        can_data10[0] = Receive_message.data[0];
        can_data10[1] = Receive_message.data[1];
        can_data10[2] = Receive_message.data[2];
        can_data10[3] = Receive_message.data[3];
        can_data10[4] = Receive_message.data[4];
        can_data10[5] = Receive_message.data[5];
        can_data10[6] = Receive_message.data[6];
        can_data10[7] = Receive_message.data[7];
        status10 = true;

    }







}