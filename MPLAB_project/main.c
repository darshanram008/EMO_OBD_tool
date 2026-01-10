/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F27Q84
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include "state_machine.h"
#include "pic18f27q84.h"
#include "BMS_data_handle.h"

char AT_command[8];
char command[150];

void main(void) {



    // baud_eeprom = DATAEE_ReadByte(0x00);
labelreset:
    GUI_baud_rate = 1;
    SYSTEM_Initialize();

    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
   // UART2_SetRxInterruptHandler(UART2_Receive_ISR);
    // UART2_SetTxInterruptHandler(UART2_Write);
    for (int i = 0; i < 20; i++) {
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




    for (int i = 0; i < 150; i++) {
        command[i] = '\0';
    }

    for (int i = 0; i < 8; i++) {
        AT_command[i] = '\0';
    }

    
    __delay_ms(5000);



     OBD_reset = false;
    state = GUI_disconnected;


    while (1) {

      

        current_state();

    }


    // Add your application code
}

