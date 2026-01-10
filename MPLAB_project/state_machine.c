#include "state_machine.h"

#include "GUI_data_handle.h"


void comns_test();
const char test[] = "AT+TEST\r\n";
uint8_t state;
bool handshake = false;


int current_state(){
    
    switch(state) {
        
        case GUI_disconnected:
            handshake = false;
            comns_test();
           
            //do something
            break;

        case GUI_connected:
            OBD_event = default_event;
            command_service();
            events();
            
            // do something 
            break;
            
       
            
            
              
    }
    return 0;
}
void comns_test()
{
   
   
    
     
    
    if(handshake == false) {
      IO_RB0_SetLow();
      IO_RC5_SetLow();
      
        uart_transmit(test);
        command_service();
   
     }
}

