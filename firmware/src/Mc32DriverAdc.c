/*--------------------------------------------------------*/
//  Mc32DriverAdc.c
/*--------------------------------------------------------*/
//	Description :	Fonctions pour gestion ADC 10 bits
//                      en mode scan
//	Auteur 		: 	C. HUBER
//
//	Version		:	V1.0
//	Compilateur	:	XC32 V1.33 & Harmony 1.00
//
//      BSP             :  pic32mx_skes
//
// Revu / modifié:
//      création 18.09.2014 CHR
/*--------------------------------------------------------*/

#include "system_config.h"
#include "Mc32DriverAdc.h"
#include "peripheral/adc/plib_adc.h"
//#include "peripheral/adc/processor/adc_p32mx130f064d.h"



// Create the list of  channels to scan
// Bit a 1  SCAN Bit0 = AN0
#define configscan  0x0040       // SCAN AN2 + 3 + 4
                    

/*--------------------------------------------------------*/
// Fonction BSP_InitADC10
/*--------------------------------------------------------*/

void BSP_InitADC10(void)
{
    PLIB_ADC_InputScanMaskAdd(ADC_ID_1, configscan) ;   // liste des AN ? scanner
    PLIB_ADC_ResultFormatSelect(ADC_ID_1, ADC_RESULT_FORMAT_INTEGER_16BIT);
    PLIB_ADC_ResultBufferModeSelect(ADC_ID_1, ADC_BUFFER_MODE_TWO_8WORD_BUFFERS);
    PLIB_ADC_SamplingModeSelect(ADC_ID_1, ADC_SAMPLING_MODE_MUXA);
 
    PLIB_ADC_ConversionTriggerSourceSelect(ADC_ID_1, ADC_CONVERSION_TRIGGER_INTERNAL_COUNT);
    PLIB_ADC_VoltageReferenceSelect(ADC_ID_1, ADC_REFERENCE_VDD_TO_AVSS );
    PLIB_ADC_SampleAcquisitionTimeSet(ADC_ID_1, 0x1F);
    PLIB_ADC_ConversionClockSet(ADC_ID_1, SYS_CLK_FREQ, 32);
 
    // Rem CHR le nb d'?chantillon par interruption doit correspondre au nb d'entr?es
    // de la liste de scan
    PLIB_ADC_SamplesPerInterruptSelect(ADC_ID_1, ADC_1SAMPLE_PER_INTERRUPT);
    PLIB_ADC_MuxAInputScanEnable(ADC_ID_1);
 
    // Enable the ADC module
    PLIB_ADC_Enable(ADC_ID_1);
 }

/*--------------------------------------------------------*/
// Fonction ReadAllADC
/*--------------------------------------------------------*/

S_ADCResults BSP_ReadAllADC()
{
    S_ADCResults result;
    
    ADC_RESULT_BUF_STATUS BufStatus;
    
    // Stop sample/convert
    PLIB_ADC_SampleAutoStartDisable(ADC_ID_1);
 
    // traitement avec buffer altern?

    BufStatus = PLIB_ADC_ResultBufferStatusGet(ADC_ID_1);
    
    if (BufStatus == ADC_FILLING_BUF_0TO7) 
    {
        result.Chan0 = PLIB_ADC_ResultGetByIndex(ADC_ID_1, 8);
    }
    else 
    {
        result.Chan0 = PLIB_ADC_ResultGetByIndex(ADC_ID_1, 0);
    }
    // Auto start sampling

    PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
    
    return result;
}
