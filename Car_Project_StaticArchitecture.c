/*
 * Car_Project_StaticArchitecture.c
 *
 * Created: 2/16/2020 4:15:26 PM
 *  Author: Reem
 */ 


#include "DIO.h"
#include "PWM.h"
#include "ICU.h"
#include "motor.h"
#include "car_sm.h"
#include "timerDelay.h"

int main(void)
{
	
	DIO_Cfg_s astr_dioCfg;
	

	astr_dioCfg.GPIO = GPIOB;
	astr_dioCfg.pins = BIT4|BIT5|BIT6|BIT7;
	astr_dioCfg.dir  = OUTPUT;
	
	DIO_init(&astr_dioCfg);
	

	Car_SM_Init();
	
  while(1)
    {
		
		Car_SM_Update();
		
		
		
    }
	
}