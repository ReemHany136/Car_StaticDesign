/*
 * car_sm.c
 *
 * Created: 2/16/2020 4:21:29 PM
 *  Author: Reem
 */ 
#include "car_sm.h"




/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
*/
ERROR_STATUS Car_SM_Init(void){
	ERROR_STATUS status;
	status =Steering_Init();
	if(status!=E_NOK){
		status =Us_Init();
	}
	return status;
}

/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
*/
ERROR_STATUS Car_SM_Update(void){
	uint8_t CurrentState =FORWARD_STATE,status;
	delayMsTimer(500);
	uint16_t Distance;
	Steering_SteerCar(CAR_STOP,0);
	while(CurrentState != STOP_STATE){
		
		//Read the distance
		status = Us_Trigger();
		status = Us_GetDistance(&Distance);
		delayMsTimer(100);
		
		//in case status is error stop the car
		if(status == E_NOK){
			CurrentState =STOP_STATE;
		}
		
		//In case the distance is greater than 30 cm the car will move forward
		else if(Distance > 45){
			CurrentState =FORWARD_STATE;
		}
		
		//In case the distance is in range 25| 30 cm the car will turn
		else if(Distance >=30  && Distance <=45){
			CurrentState = TURN_STATE;
		}
		
		//In case the distance is smaller than 25 cm the car will move backwards then turn
		else{
			CurrentState = BACKWARD_TURN_STATE;
		}
		
		//Switch the states
		switch(CurrentState)
		{
			/*************************************************/
			case FORWARD_STATE:
			Steering_SteerCar(CAR_FORWARD,25);
			break;
			
			/*************************************************/
			case TURN_STATE:
			Steering_SteerCar(CAR_LEFT,25);
			delayMsTimer(200);
			break;
			
			
			/*************************************************/
			case BACKWARD_TURN_STATE:
			
			//Go backwards until the distance increases to be greater than or equal 25
			Steering_SteerCar(CAR_BACKWARD,25);
			
			break;
			
			/*************************************************/
			
			//In case there is an error the car will stop 
			case STOP_STATE:
			Steering_SteerCar(CAR_STOP,0);
			break;
			
			/*************************************************/
			default:
			return E_NOK;
			break;
		}
		
	}
	return status;
}