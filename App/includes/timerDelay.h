/*
 * timerDelay.h
 *
 * Created: 2/17/2020 11:43:32 AM
 *  Author: Reem
 */ 


#ifndef TIMERDELAY_H_
#define TIMERDELAY_H_
#include "Timer.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define delayChannel   TIMER_CH2
#define delayPrescale  TIMER_PRESCALER_64
#define delayCounts    250

/************************************************************************/
/*				 PROTOTYPES			        */
/************************************************************************/

/*
*Input: delay time in millis 
*Output: No output
*Description: delay time in millis 
*/
extern void delayMsTimer(uint32_t delayMs);
extern void softwareDelayMs_(uint32_t delayTime);

#endif /* TIMERDELAY_H_ */