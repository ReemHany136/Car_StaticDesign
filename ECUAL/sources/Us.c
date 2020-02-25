/*
 * Us.c
 *
 * Created: 2/16/2020 4:32:03 PM
 *  Author: Reem
 */ 
#include "ICU.h"
#include "Us.h"

/****************************************************************************************************
 * Function Name   : Us_Init.                                                                       *
 * Input Parameters : None.                                                                         *                                                                   *                                                                                                *
 * Output Parameters: None.                                                                         *
 * In_Out Parameters: None.                                                                         *
 * Return           : ERROR_STATUS.                                                                 *
 * Description      : This function initiate the UltraSonic by set the value of the Dio_Dirction of *
 *                    the pin trigger as an output then initiate the ICU to use it to calculate the *
 *                    On time of the ECHO signal.                                                   *
 *                                                                                                  *
 *                                                                                                  *
 ***************************************************************************************************/



ERROR_STATUS Us_Init(void){
	Icu_cfg_s astr_icuCfg;
	DIO_Cfg_s astr_dioCfg;
	
	astr_icuCfg.ICU_Ch_Timer = ICU_TIMER_CH0;
	astr_icuCfg.ICU_Ch_No =ICU_CH2;
	if(Icu_Init(&astr_icuCfg) == E_NOK){
		return E_NOK;
	}
	else{
		astr_dioCfg.GPIO =TRIGGER_PORT;
		astr_dioCfg.pins =TRIGGER_PIN;
		astr_dioCfg.dir  =OUTPUT;
		
		DIO_init(&astr_dioCfg);
		
		
		return E_OK;
	}
	
}




/**************************************************************************************************
 * Function Name    : Us_Trigger.                                                                 *
 * Input Parameters : None.                                                                       *                                                                              *                                                                                                *
 * Output Parameters: None.                                                                       *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS                                                                *
 * Description      : This function send the  pulse, to trigger pin of the UltraSonic to enhance  *
 *                    the  ECUO signal                                                            *
 *************************************************************************************************/


ERROR_STATUS Us_Trigger(void){
	
	if(DIO_Write(TRIGGER_PORT,TRIGGER_PIN,HIGH) == E_NOK)
		return E_NOK;

	delayMsTimer(1);
	
	if(DIO_Write(TRIGGER_PORT,TRIGGER_PIN,LOW) == E_NOK)
		return E_NOK;
		
		
	return E_OK;
}





/**************************************************************************************************
 * Function Name    : Us_GetDistance.                                                             *
 * Input Parameters : pointer to uint16 that will return the distance on it.                      *                                                                                              *                                                                                                *
 * Output Parameters: Distance.                                                                   *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS.                                                               *
 * Description      : This function get the time of high signal of ECUO pulse, then               *
 *                    use it to calculate the distance, then return the value in the              *
 *                    Distance parameter.                                                         *
 *************************************************************************************************/

ERROR_STATUS Us_GetDistance(uint16_t *Distance){
	if(Distance != NULL){
		uint32_t au32_timeInMicros;
		if(Icu_ReadTime(ICU_TIMER_CH0,ICU_RISE_TO_FALL,&au32_timeInMicros) == E_OK){
			
			(*Distance) = au32_timeInMicros / 58;
			
			
			return E_OK;
		}
		else{
			return E_NOK;
		}
	}
	else{
		return E_NOK;
	}
	
}