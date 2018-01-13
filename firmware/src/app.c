/* 
 * File:   app.c
 * Author: Jake OBrien
 *
 * Created on November 30, 2017, 11:54 AM
 */
 #include <string.h>
#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define RA7 (0x1 << 7)
#define STACK_SIZE 400
BaseType_t xTimer1Started, xReturned;
TimerHandle_t t;
static float AN0_AVG, AN1_AVG, DELTA_AN0, DELTA_AN1;
char val;
bool roll;
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void TimerCallBack(TimerHandle_t xTimer)
{
    roll = true;
        
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    dbgSetup();
    rec = 'f';
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            roll = false;
            bool appInitialized = true;
            dbgOutputLoc(DLOC_ISR_END | DLOC_UART );
            xQueue1 = xQueueCreate( 50, sizeof( struct AMessage * ) );
            TickType_t xTimeInTicks = pdMS_TO_TICKS( 250 );
            t = xTimerCreate("Timer", xTimeInTicks, pdTRUE, 0, TimerCallBack);
            if( t != NULL ) {
                 xTimer1Started = xTimerStart( t, 0 );
                 
                 if( xTimer1Started != pdPASS ) {
                 }
            }
            TaskHandle_t xHandle = NULL;
            val = 'f';
            /* Create the task, storing the handle. */
            xReturned = xTaskCreate(
                            vTaskCode,       /* Function that implements the task. */
                            "QTask",          /* Text name for the task. */
                            STACK_SIZE,      /* Stack size in words, not bytes. */
                            ( void * ) 1,    /* Parameter passed into the task. */
                            (1|portPRIVILEGE_BIT),/* Priority at which the task is created. */
                            &xHandle );      /* Used to pass out the created task's handle. */
            
            if (appInitialized)
            {
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            break;
        }
        /* The default state should never be executed. */
        default:
        {
            break;
        }
    }
}

void vTaskCode( void * pvParameters )
{
    dbgOutputLoc(DLOC_TASK_BEGIN | DLOC_UART);
    configASSERT(((unit32_t)pvParameters) == 1);
    const struct AMessage *pxRxedMessage;
    float range_delta = 2;
    int gcount = 0;
    int hcount = 0;
   
    dbgOutputLoc(DLOC_TASK_WHILE | DLOC_UART);
    //dbgAssert(0); //Demonstrate error
    while(1) {
		roll = false; //reset timer call
        while(!DRV_ADC_SamplesAvailable() || !PLIB_USART_TransmitterIsEmpty(USART_ID_1));
		//if timer has trigger and queue is full trigger (blocking)
        if(roll && xQueueReceive(xQueue1, &(pxRxedMessage), portMAX_DELAY)) 
        {
            dbgOutputLoc(DLOC_TASK_QEND | DLOC_UART);
                
			//Sensor Processing:
			//1. convert digital value to voltage
			//2. convert voltage to cm using sensor voltage formula
			//3. find distance delta compared to last distance
			//4. save current distance for next delta
            AN0_AVG = ((pxRxedMessage->AN0_Data / 8) * 3.3)/1023;
            AN0_AVG = 47.31/(AN0_AVG - 0.085);
            DELTA_AN0 = fabs(LD_AN0 - AN0_AVG);
            LD_AN0 = AN0_AVG;
            AN1_AVG = ((pxRxedMessage->AN1_Data / 8) * 3.3)/1023;
            AN1_AVG = 2076/((AN1_AVG * 200) - (11));
            DELTA_AN1 = fabs(LD_AN1 - AN1_AVG);
            LD_AN1 = AN1_AVG;
            range_delta = 2.8;
            if(AN0_AVG < 75 && DELTA_AN0 < 5.5 && DELTA_AN0 > range_delta ) {
                //jsonFormat(0,1, DELTA_AN0, json);
				//dbgUARTstr(json);
                dbgUARTVal('C'); //denotes computer moved
            }
            if(DELTA_AN1 > 2 && gcount >= 1) {
                //jsonFormat(1,1, DELTA_AN1, json);
                //dbgUARTstr(json);
                dbgUARTVal('J'); //denoted computer goal
                gcount = 0;
            }
            else if(DELTA_AN1 > 2) {
                gcount++;
            }
            else {
                gcount = 0;
            }
            AN0_AVG = 0;
            AN1_AVG = 0;
            DELTA_AN0 = 0;
            DELTA_AN1 = 0;
            //ledTrig(true);
            PLIB_ADC_SampleAutoStartEnable(DRV_ADC_ID_1);
            PLIB_ADC_ConversionStopSequenceEnable(DRV_ADC_ID_1);

        }
        
    }
}

