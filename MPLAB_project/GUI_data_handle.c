#include "GUI_data_handle.h"
#include "state_machine.h"

#include "OBD_tool_events.h"
 
char timeout_msg[] = "AT+TIME\r\n", gui_ack[10] = "AT+DONE\r\n", AT_error[] = "AT+ERRR\r\n";
uint8_t i,m,k,count;
char command[150];
uint8_t rx_buffer[250];
volatile uint8_t rx_buffer_index = 0;
volatile bool rx_receive_complete = false;
long int start_point = 0, end_point = 0;
char AT_command[8];
volatile bool OBD_reset = false;





void uart_transmit(char *uart_data) {

    for (i = 0; i < strlen(uart_data); i++) {
        UART2_Write(uart_data[i]);
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
    char* pos7 = strstr(command, "AT+BOOT=");
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

    }  else if (pos7 != NULL) {
        if (pos5 != NULL) {

            end_point = (pos5 - command);
            start_point = (pos7 - command);
            for (i = start_point; i < (start_point + 8); i++) {
                AT_command[m] = command[i];
                m++;
            }
        }
    }else {
        uart_transmit(AT_error);
    }

}
void command_service(){                                                                                                                                          
      if (rx_receive_complete == true) {

        // IO_RB0_SetLow();
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
                        
                        handshake = true;
                        state = GUI_connected;
                    }
                     if ((command[start_point + 8] == 'C') & (command[start_point + 9] == 'C')) {
                        
                        handshake = false;
                        
                        OBD_reset = true;
                    }


                }
            }
        
        
        if (!(strncmp(AT_command, "AT+BAUD=", 8))) //SERIAL BAUD RATE
        {
            
            OBD_event = GUI_baud_rate_change_event;
            
        }
        if (!(strncmp(AT_command, "AT+CANB=", 8))) //CAN BAUD RATE
        {
            
            OBD_event = BMS_baud_rate_change_event;
           
        }

        if (!(strncmp(AT_command, "AT+DATA=", 8))) {
            
            OBD_event = sd_data_download_event;
        }

        if (!(strncmp(AT_command, "AT+REST=", 8))) //SOFTWARE RESET
        {           
            OBD_event = BMS_software_rst_event;            
        }
        
         if (!(strncmp(AT_command, "AT+BOOT=", 8))) //SERIAL BAUD RATE
        {       
            OBD_event = BMS_bootloader_event;
            
        }

        if (!(strncmp(AT_command, "AT+CONF=", 8))) //CONFIG DATA
        {               
            can_rx_receive_flag = false;
          
            count = 0;
            k = 0;
            for (i = start_point; i < end_point; i++) {
                count++;

            }

            switch (count) {
                case 68:
                    
                    OBD_event = BMS_upl_config_event;
                  
                    break;
                    /******************************config file BMS to gui***************************************************/
                case 9:
                    
                    OBD_event = BMS_get_config_event;
                    
                    break;

                default:uart_transmit(AT_error);
            }
        }


        for (i = 0; i < 8; i++) {
            AT_command[i] = '\0';
        }


        rx_receive_complete = false;
    }
}
