/* 
 * File:   debug.h
 * Author: Jake OBrien
 *
 * Created on November 30, 2017, 11:54 AM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#include <stdio.h>
#include <string.h>
#include "system_config.h"
#include "system_definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Debug Location Flags*/
    /* more defintions to be added during development*/
    #define DLOC_ISR_BEGIN         0x0
    #define DLOC_ISR_END           0x1
    #define DLOC_ISR_QBEGIN        0x2
    #define DLOC_ISR_QEND          0x3
    #define DLOC_TASK_BEGIN        0x4
    #define DLOC_TASK_WHILE        0x5
    #define DLOC_TASK_QBEGIN       0x6
    #define DLOC_TASK_QEND         0x7
    #define DLOC_CALLBACK_BEGIN    0x8
    #define DLOC_CALLBACK_END      0x9
    #define DLOC_CALLBACK_QBEGIN   0xa
    #define DLOC_CALLBACK_QEND     0xb
    /**0xc to 0xf reserved for locations*/
    /*Debug Option Flags*/
    /* or(|) flags with location flags above to specify type*/
    /* add your own if there is a location that is not written here*/
    #define DLOC_UART   0x10
    #define DLOC_TIMER  0x20
    #define DLOC_MOTION 0x30

    /* Pin Definition */
    /**subject to move to a global file if needed**/

    /** PIC I/0 **/
    #define RE7    (0x1 << 7) //30
    #define RE6    (0x1 << 6)
    #define RE5    (0x1 << 5)
    #define RE4    (0x1 << 4)
    #define RE3    (0x1 << 3)
    #define RE2    (0x1 << 2)
    #define RE1    (0x1 << 1)
    #define RE0    (0x1)     //37
    /** Debug I/O **/
    #define RG6    (0x1 << 6)
    #define RG7    (0x1 << 7)
    #define RD8    (0x1 << 8)
    #define RF1    (0x1 << 1)
    #define RA10   (0x1 << 10)
    #define RB12   (0x1 << 12)
    #define RB11   (0x1 << 11)
    #define RD10   (0x1 << 10)
    /** Error LED **/
    #define RA3    (0x1 << 3)
    /**define other pins here*/
    
    #define BUFF_SIZE 5
    #define ADC_SIZE 2
 
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */
    
     
     struct AMessage
    {
        char ucMessageID;
        float AN0_Data;
        float AN1_Data;
    } xMessage;
    
    typedef struct AMessage AMessage;

    QueueHandle_t xQueue1;
    float LD_AN0, LD_AN1;

    // *****************************************************************************
    void dbgSetup( void );
    void dbgOutputVal(unsigned char outVal);
    void dbgUARTVal(unsigned char outVal);
    void dbgUARTstr(char* outStr);
    void dbgOutputLoc(unsigned char outVal);
    void dbgAssert(int assert);
    void recUARTVal( void ) __attribute__ ((deprecated));
    void jsonFormat(int type, int side, int delta, char * str);
    void ledTrig( bool b);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */

