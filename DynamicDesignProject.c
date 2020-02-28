/*
 * DynamicDesignProject.c
 *
 * Created: 2/24/2020 11:37:59 AM
 *  Author: Reem
 */ 
#include "DIO.h"
#include "TMU.h"

void Consumer1(void);
void Consumer2(void);
void Consumer3(void);

int main(void)
{
	DIO_Cfg_s astr_DIO;
	TMU_TASK_Cfg astr_TMU_TASK;
	TMU_TASK_Cfg astr_TMU_TASK1;
	TMU_Cfg astr_TMUCfg;
	
	astr_DIO.GPIO = GPIOB;
	astr_DIO.pins = BIT4|BIT5|BIT6;
	astr_DIO.dir = OUTPUT;
	DIO_init(&astr_DIO);
	
	
	astr_TMUCfg.TIM_Ch = TMU_TIMER_CH2;
	TMU_Init(&astr_TMUCfg);
	
	TMU_DeInit();
	
	
	astr_TMUCfg.TIM_Ch = TMU_TIMER_CH2;
	TMU_Init(&astr_TMUCfg);
	
	
	astr_TMU_TASK.Task_id = 1;
	astr_TMU_TASK.TMU_MODE = PERIODIC;
	astr_TMU_TASK.TMU_Cbk_ptr =Consumer1;
	astr_TMU_TASK.Delay_TimeMs = 20;
	
	TMU_Start_Timer(&astr_TMU_TASK);
	
	

	astr_TMU_TASK1.Task_id = 2;
	astr_TMU_TASK1.TMU_MODE = PERIODIC;
	astr_TMU_TASK1.TMU_Cbk_ptr =Consumer2;
	astr_TMU_TASK1.Delay_TimeMs = 10;
	
	TMU_Start_Timer(&astr_TMU_TASK1);
	
	TMU_TASK_Cfg astr_TMU_TASK2;
	
	astr_TMU_TASK2.Task_id = 3;
	astr_TMU_TASK2.TMU_MODE = ONE_SHOT;
	astr_TMU_TASK2.TMU_Cbk_ptr =Consumer3;
	astr_TMU_TASK2.Delay_TimeMs = 5;
	
	TMU_Start_Timer(&astr_TMU_TASK2);
	
    while(1)
    {
        TMU_Dispatcher();
    }
}

void Consumer1(void){
	DIO_Toggle(GPIOB,BIT4);
}
void Consumer2(void){
	DIO_Toggle(GPIOB,BIT5);
}
void Consumer3(void){
	DIO_Toggle(GPIOB,BIT6);
}