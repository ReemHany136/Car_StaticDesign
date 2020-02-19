/*
 * timerDelay.c
 *
 * Created: 2/17/2020 12:21:17 PM
 *  Author: Reem
 */ 
#include "DIO.h"
#include "timerDelay.h"


/*
*Input: delay time in millis
*Output: No output
*Description: delay time in millis
*/
void delayMsTimer(uint32_t delayMs){
	uint32_t au32_iterator;
	uint8_t au8_FLAG=FALSE;
	Timer_cfg_s astr_TimerCfg;
	astr_TimerCfg.Timer_CH_NO = DELAY_CHANNEL;
	astr_TimerCfg.Timer_Mode = TIMER_MODE;
	astr_TimerCfg.Timer_Polling_Or_Interrupt=TIMER_POLLING_MODE;
	astr_TimerCfg.Timer_Prescaler = delayPrescale; 
	
	Timer_Init(&astr_TimerCfg);
	
	Timer_Start(DELAY_CHANNEL,DELAY_COUNTS);	
	for(au32_iterator = 0 ; au32_iterator < delayMs ; au32_iterator++){
		
		//Timer_GetStatus(delayChannel,&FLAG);
		while (!au8_FLAG)
		{
			Timer_GetStatus(DELAY_CHANNEL,&au8_FLAG);
		}
		au8_FLAG = FALSE;
	}Timer_Stop(DELAY_CHANNEL);
	
	
}

/*
*Input: delay time in millis
*Output: No output
*Description: delay time in millis
*/
void softwareDelayMs_(uint32_t delayTime){
	uint32_t au32_iterator_i ,au32_iterator_j ;
	
	for (au32_iterator_i=0;au32_iterator_i<400;au32_iterator_i++)
	{
		for(au32_iterator_j=0;au32_iterator_j<delayTime;au32_iterator_j++);
	}
}