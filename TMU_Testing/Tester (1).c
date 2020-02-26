

#include "TMU.h"

#define TMU_BUFFER_SIZE  5

#define INVALID_PARAMETER_TEST 5



void toggle1(void){

    printf("toggled") ;

}

void Test_TMU_Start(void){

    sint16_t state_error = E_OK ;


    printf("Testing TMU_Start_Timer Function\n");
    printf("_____________\n");

    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: start before init.
    * Expected OUTPUT: TMU_module_error + MODULE_UNINTIALIZED
    ************************************************************************************/




    TMU_Cfg_s TMU_cfg1 ;
	TMU_cfg1.Timer_channel = TMU_TIMER_CH2 ;
	TMU_cfg1.TMU_Reselution = reselution_one_mili ;

	TMU_Init(&TMU_cfg1);


    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: null pointer to the struct
    * Expected OUTPUT: TMU_module_error +NULL_PTR_ERROR
    ************************************************************************************/
    TMU_tsak_s *PTR_TMU_task1=NULL ;

    state_error = 	TMU_Start_Timer(PTR_TMU_task1) ;


    printf("Test case 1 : ");

    if(state_error != E_OK)
        printf("passed \n");
    else
        printf("failed \n");

    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: invalid parameter (mode != periodic or mode != one shot )
    * Expected OUTPUT: TMU_module_error + INVALID__PARAMETER
    ************************************************************************************/
    TMU_tsak_s TMU_task2 ;
    TMU_task2.delay_time = 1000;
	TMU_task2.Ptr_FunctionCall = toggle1;
	TMU_task2.Task_ID = 2;
	TMU_task2.TMU_mode = INVALID_PARAMETER_TEST ;

    state_error = 	TMU_Start_Timer(&TMU_task2) ;


    printf("Test case 2 : ");

    if(state_error != E_OK)
        printf("passed \n");
    else
        printf("failed \n");




    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: Null pointer in the parameter Ptr_FunctionCall
    * Expected OUTPUT: TMU_module_error + INVALID__PARAMETER
    ************************************************************************************/
    TMU_tsak_s TMU_task3 ;
    TMU_task3.delay_time = 1000;
	TMU_task3.Ptr_FunctionCall = NULL;
	TMU_task3.Task_ID = 3;
	TMU_task3.TMU_mode = PERIODIC ;

    state_error = 	TMU_Start_Timer(&TMU_task3) ;


    printf("Test case 3 : ");

    if(state_error != E_OK)
        printf("passed \n");
    else
        printf("failed \n");




    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: valid parameters
    * Expected OUTPUT: E_OK
    ************************************************************************************/
    TMU_tsak_s TMU_task4 ;
    TMU_task4.delay_time = 1000;
	TMU_task4.Ptr_FunctionCall = toggle1;
	TMU_task4.Task_ID = 4;
	TMU_task4.TMU_mode = PERIODIC ;

    state_error = 	TMU_Start_Timer(&TMU_task4) ;


    printf("Test case 4 : ");

    if(state_error == E_OK)
        printf("passed \n");
    else
        printf("failed \n");


    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: same task ID
    * Expected OUTPUT: TMU_module_error + INVALID__PARAMETER
    ************************************************************************************/
    TMU_tsak_s TMU_task5 ;
    TMU_task5.delay_time = 1000;
	TMU_task5.Ptr_FunctionCall = toggle1;
	TMU_task5.Task_ID = 4;
	TMU_task5.TMU_mode = PERIODIC ;

    state_error = 	TMU_Start_Timer(&TMU_task5) ;


    printf("Test case 5 : ");

    if(state_error != E_OK)
        printf("passed \n" );
    else
        printf("failed \n");



    /************************************************************************************
    * Target Function: TMU_Start_Timer
    * Test Case: full buffer
    * Expected OUTPUT: TMU_module_error + FULL_BUFFER
    ************************************************************************************/
    TMU_tsak_s TMU_task6 ;
    TMU_task6.delay_time = 1000;
	TMU_task6.Ptr_FunctionCall = toggle1;
	TMU_task6.Task_ID = 6;
	TMU_task6.TMU_mode = PERIODIC ;

    state_error = 	TMU_Start_Timer(&TMU_task6) ;
    state_error = 	TMU_Start_Timer(&TMU_task6) ;

    state_error = 	TMU_Start_Timer(&TMU_task6) ;

    state_error = 	TMU_Start_Timer(&TMU_task6) ;

    TMU_Stop_Timer(&TMU_task6) ;
    TMU_Stop_Timer(&TMU_task6) ;

    printf("Test case 6 : ");

    if(state_error != E_OK)
        printf("passed \n" );
    else
        printf("failed \n");



}


void Test_TMU_Stop(void){

    sint16_t state_error = E_OK ;

    printf("\n \n");
     printf("Testing TMU_Stop_Timer Function\n");
    printf("_____________\n");


    TMU_Cfg_s TMU_cfg1 ;
	TMU_cfg1.Timer_channel = TMU_TIMER_CH2 ;
	TMU_cfg1.TMU_Reselution = reselution_one_mili ;

	TMU_Init(&TMU_cfg1);



    /************************************************************************************
    * Target Function: TMU_Stop_Timer
    * Test Case: stop task with valid ID
    * Expected OUTPUT: TMU_module_error + INVALID__PARAMETER
    ************************************************************************************/
    TMU_tsak_s TMU_task5 ;
    TMU_task5.delay_time = 1000;
	TMU_task5.Ptr_FunctionCall = toggle1;
	TMU_task5.Task_ID = 7;
	TMU_task5.TMU_mode = PERIODIC ;

    state_error = 	TMU_Start_Timer(&TMU_task5) ;

    //state_error = 	TMU_Stop_Timer(&TMU_task5) ;
    printf("Test case 7 : ");

    if(state_error == E_OK)
        printf("passed  \n" );
    else
        printf("failed \n");


    /************************************************************************************
    * Target Function: TMU_Stop_Timer
    * Test Case: stop task with invalid ID
    * Expected OUTPUT: TMU_module_error + INVALID__PARAMETER
    ************************************************************************************/
    TMU_tsak_s TMU_task6 ;
    TMU_task6.delay_time = 1000;
	TMU_task6.Ptr_FunctionCall = toggle1;
	TMU_task6.Task_ID = 8;
	TMU_task6.TMU_mode = PERIODIC ;

    state_error = 	TMU_Start_Timer(&TMU_task6) ;

    TMU_tsak_s TMU_task7 ;
    TMU_task7.delay_time = 1000;
	TMU_task7.Ptr_FunctionCall = toggle1;
	TMU_task7.Task_ID = 10;
	TMU_task7.TMU_mode = PERIODIC ;

    state_error = 	TMU_Stop_Timer(&TMU_task7) ;
    printf("Test case 8 : ");

    if(state_error != E_OK)
        printf("passed \n" );
    else
        printf("failed \n");



}


