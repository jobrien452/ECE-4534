#include "debug.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
DRV_HANDLE myUSARTHandle;

/* ************************************************************************** */
void dbgSetup() {
    TRISECLR |= RE7 | RE6 | RE5 | RE4 | RE3 | RE2| RE1 | RE0;
    TRISGCLR |= RG6 | RG7;
    TRISDCLR |= RD8 | RD10;
    TRISFCLR |= RF1;
    TRISACLR |= RA10;
    TRISBCLR |= RB11 | RB12;
    //myUSARTHandle = 
    //DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
    DRV_ADC_Open();
    DRV_ADC_ChannelScanInputsAdd(ADC_INPUT_SCAN_AN0 | ADC_INPUT_SCAN_AN1);
    //PLIB_ADC_MuxAInputScanEnable(DRV_ADC_ID_1);
    //DRV_ADC_Start();
}

void dbgOutputLoc(unsigned char outVal) {
    PORTE &= ~(0xFF);
    if(outVal & 0x1) {
        PORTE |= RE7;
    }
    if(outVal & 0x2) {
        PORTE |= RE6;
    }
    if(outVal & 0x4)  {
        PORTE |= RE5;
    }
    if(outVal & 0x8) {
        PORTE |= RE4;
    }
    if(outVal & 0x10)  {
        PORTE |= RE3;
    }
    if(outVal & 0x20) {
        PORTE |= RE2;
    }
    if(outVal & 0x40) {
        PORTE |= RE1;
    }
    if(outVal & 0x80)  {
        PORTE |= RE0;
    }
}

void dbgUARTVal(unsigned char outVal) {
    
    //while(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL){};
    if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus())) {
        DRV_USART0_WriteByte(outVal);
        
    }
}

void jsonFormat(int type, int side, int delta, char * str) {
    
    char buff[15];
    strcpy(str, "{\n\r\t");
    
    if(type) {
        strcat(str, "\"type\" : \"Goal\",\n\r\t");
    }
    else {
        strcat(str, "\"type\" : \"Movement\",\n\r\t");
    }
    
    if(side) {
        strcat(str, "\"side\" : \"COM\",\n\r\t");
    }
    else {
        strcat(str, "\"side\" : \"HUMAN\",\n\r\t");
    }
    
    //if(delta > 0) {
        sprintf(buff, "\"delta\" : \"%d\"\n\r\t", delta);
        strcat(str, buff);
    //}
    
    strcat(str, "}\n\r\0");
    //pxMessage = &xMessage;
}

void dbgUARTstr(char* outStr) {
    int i;
    //PORTA = RA3;
    for(i = 0; i < strlen(outStr); i++) {
        //if(outStr[i] != "/0"){
            if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus())) {
                dbgUARTVal(outStr[i]);
            }
            else {
                i--;
            }
        //}
    }
}


void dbgOutputVal(unsigned char outVal) {
    PORTG &= ~(RG6|RG7);
    PORTD &= ~(RD8|RD10);
    PORTF &= ~RF1;
    PORTA &= ~RA10;
    PORTB &= ~(RB11|RB12);
    if(outVal & 0x1) {
        PORTG |= RG6;
    }
    if(outVal & 0x2) {
        PORTG |= RG7;
    }
    if(outVal & 0x4)  {
        PORTD |= RD8;
    }
    if(outVal & 0x8) {
        PORTF |= RF1;
    }
    if(outVal & 0x10)  {
        PORTA |= RA10;
    }
    if(outVal & 0x20) {
        PORTB |= RB12;
    }
    if(outVal & 0x40) {
        PORTB |= RB11;
    }
    if(outVal & 0x80)  {
        PORTD |= RD10;
    }
}

void dbgAssert(int assert) {
    if(!assert) {
        //Disable_global_interrupt();
        vTaskSuspendAll();
        PORTA |= RA3;
    }
}
/*TODO enable adc with auto sample enable plib function*/
void recUARTVal() {
     int i = 0;
     //unsigned char msg[20] = "11 0";
     struct AMessage *pxMessage;
     xMessage.ucMessageID = 0;
     
     unsigned char val;
     if(!DRV_USART_ReceiverBufferIsEmpty(myUSARTHandle)){
         val = DRV_USART_ReadByte(myUSARTHandle);
         //trigger interrupt if wifly receives green light or go signal from user
         if(val == 'G') {
             /*xMessage->ucData = "COM"
             pxMessage = &xMessage;
              xQueueSend( xQueue1, ( void * ) &pxMessage, ( TickType_t ) 0 );*/
             //PLIB_INT_SourceFlagSet(INT_ID_0, INT_SOURCE_EXTERNAL_1);
         }
         //trigger interrupt if wifly receives red light or go signal from user
         else if(val == 'R') {
             /*strcpy( xMessage.ucData, val);
             pxMessage = &xMessage;
              xQueueSend( xQueue1, ( void * ) &pxMessage, ( TickType_t ) 0 );*/
             //PLIB_INT_SourceFlagSet(INT_ID_0, INT_SOURCE_EXTERNAL_1);
         }
         dbgUARTVal(val);
         //msg[2] = val;
         /*while(msg[i] != '\0') {
             
             i++;
         }*/
         //dbgUARTVal('x');
     }
}

void ledTrig(bool b) {
    if(b) {
        PORTA = RA3;
    }
    else {
        PORTA = (0xFF & ~RA3);
    }
}

/* *****************************************************************************
 End of File
 */

