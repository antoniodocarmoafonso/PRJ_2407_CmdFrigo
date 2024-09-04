/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include "app.h"
#include "C:\microchip\harmony\v2_06\framework\peripheral\oc\processor\oc_p32mx130f064d.h"
#include "Mc32DriverAdc.h"
#include "peripheral/adc/plib_adc.h"
#include "MC32DriverLcd.h"
#include "Mc32Delays.h"
#include "Mc32_I2cUtilCCS.h"
#include "chu.h"
#include "modules\RFIDB1ClientInterface.h"
#include "modules\RFIDB1ClientProtocol.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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
S_ADCResults value_ADC;
RFIDB1_InterfaceT rfidInterface;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


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

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
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
            bool appInitialized = true;
            
             // Initialisation du LCD
            lcd_init();
            lcd_bl_on();

            printf_lcd("2407 Commande");
            lcd_gotoxy(1,2); 
            printf_lcd("ouverture frigo");
            lcd_gotoxy(1,3); 
            printf_lcd("Antonio Do Carmo");
            
            ON_OFF_12VOff();
            MCP_RESETOff();
            
            
            BSP_InitADC10();
            i2c_init(0);
            
            CHU_Initialize();
//            CHU_EnablePolling();
            
            DRV_TMR0_Start();
            DRV_TMR1_Start();
            
            DRV_OC0_Start();
            
            CHU_EnablePolling();
            
            if (appInitialized)
            {
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {        
            
            uint16_t sizeTest;
            
            
            float valAccu;
            
            ReadBattery();
            
            DisplayLed();
            
            //compteur 1s 
            CHU_Tasks();
            
            
            
           //prendre la valeur de l'UID 
//            cestUnTest();
//             if (CHU_NewUID())
//            {
//            
//            uint8_t uid[RFIDB1_UID_MAX_SIZE];
            
//            CHU_GetUID(uid);
//             }
            
//            CHU_EnablePolling();
//            CHU_Tasks();
//            
//            uint8_t uid[RFIDB1_UID_MAX_SIZE];
//            CHU_GetUID(uid);
//            CHU_ResetFlags();
            
//            cestUnTest();
            
            
//            OuverturePorte(valAccu);
//            
//            APP_UpdateState(APP_STATE_WAIT);
            
            break;
        }

        /* TODO: implement your application state machine.*/
        case APP_STATE_WAIT:
        {
            
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

//float ReadBattery(void)
//{
//    READ_BATOn();
//    
//    float valAccu;
//    uint16_t raw_adc_value;
//
//    value_ADC = BSP_ReadAllADC();
//
//    READ_BATOff();
//
//    raw_adc_value = value_ADC.Chan0;
//    valAccu = (3.3f * (float)raw_adc_value) / 1024.0f;
//    
//    return valAccu;
//}

void ReadBattery(void)
{
    uint16_t valADC_Niveau_Accu = 0;
    float valTensionAccu = 0.0;
    
    READ_BATOn();
    //delay_usCt(5);
    PLIB_ADC_InputScanMaskAdd(ADC_ID_1, ADC_INPUT_SCAN_AN6);
    PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
    while (!PLIB_ADC_ConversionHasCompleted(ADC_ID_1));
    valADC_Niveau_Accu = PLIB_ADC_ResultGetByIndex(ADC_ID_1, 8);
    PLIB_ADC_SampleAutoStartDisable(ADC_ID_1);

    valTensionAccu = ((valADC_Niveau_Accu * 3.3) / 1023)* 2;
    
    lcd_gotoxy(1,4); 
    printf_lcd("%3f", valTensionAccu);
    
    READ_BATOff();
}

/*    
void OuverturePorte(float valAccu)
{
    float floatWidthPulse;
    uint16_t widthPulse;
    
    floatWidthPulse = (-valAccu)/12.0f + 1;
    widthPulse = (int16_t)(floatWidthPulse * 65535);
    
//    widthPulse = (int16_t)((valAccu / 12.0f) + 1.0f);
//    widthPulse = -widthPulse;
    
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, widthPulse);
    
    
}
*/

 //Fonction pour l'update de l'état de la machine d'état
void APP_UpdateState ( APP_STATES NewState)
{
   //Actualisation de la machine d'état en fonction du paramètre d'entrée
   appData.state = NewState;
}

void DisplayLed(void)
{
    MCP_RESETOn();
    // Démarre la communication I2C
    i2c_start();
    // Envoie l'adresse I2C du capteur en mode écriture
    i2c_write(0x42);
    i2c_write(0x00);
    i2c_write(0x00);
    
    i2c_stop();
    i2c_start();
    
    i2c_write(0x42);
    i2c_write(0x09);
    i2c_write(0xFF);
    // Arrête la communication I2C
    i2c_stop();
    MCP_RESETOff();
}

/*******************************************************************************
 End of File
 */
