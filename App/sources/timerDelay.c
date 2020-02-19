/*
 * timerDelay.c
 *
 * Created: 2/17/2020 12:21:17 PM
 *  Author: Reem
 */ 
#include "DIO.h"
#include "timerDelay.h"

void delayMsTimer(uint32_t delayMs){
	uint32_t i;
	uint8_t FLAG=FALSE;
	Timer_cfg_s TimerCfg;
	TimerCfg.Timer_CH_NO = delayChannel;
	TimerCfg.Timer_Mode = TIMER_MODE;
	TimerCfg.Timer_Polling_Or_Interrupt=TIMER_POLLING_MODE;
	TimerCfg.Timer_Prescaler = delayPrescale; 
	
	Timer_Init(&TimerCfg);
	
		Timer_Start(delayChannel,delayCounts);	
	for(i = 0 ; i < delayMs ; i++){
		
		//Timer_GetStatus(delayChannel,&FLAG);
		while (!FLAG)
		{
			Timer_GetStatus(delayChannel,&FLAG);
		}
		FLAG = FALSE;
	}Timer_Stop(delayChannel);
	
	
}

void softwareDelayMs_(uint32_t delayTime){
	uint32_t i,j;
	for (i=0;i<400;i++)
	{
		for(j=0;j<delayTime;j++);
	}
}