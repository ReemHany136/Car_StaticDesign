/*
 * Steering.c
 *
 * Created: 2/16/2020 4:31:26 PM
 *  Author: Reem
 */ 
#include "Steering.h"

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */

ERROR_STATUS Steering_Init(void){
	ERROR_STATUS status;
	
	status= Motor_Init(MOTOR_1);
	status= Motor_Init(MOTOR_2);
	return status;
}

/**
 * Input: 
 * 	carDirection: The direction of the car from the predefined states of the module.
 * 	speed: The speed of the car from 0% to 100%.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the command and the speed of the car.
 * 							
 */
ERROR_STATUS Steering_SteerCar(uint8_t Steering_CarCmd, uint8_t speed){
	switch(Steering_CarCmd){
		case CAR_FORWARD:
		Motor_Direction(MOTOR_1,MOTOR_FORWARD);
		Motor_Direction(MOTOR_2,MOTOR_FORWARD);
		Motor_Start(MOTOR_1,speed);
		Motor_Start(MOTOR_2,speed);
		break;
		
		case CAR_BACKWARD:
		Motor_Direction(MOTOR_1,MOTOR_BACKWARD);
		Motor_Direction(MOTOR_2,MOTOR_BACKWARD);
		Motor_Start(MOTOR_1,speed);
		Motor_Start(MOTOR_2,speed);
		break;
		
		case CAR_LEFT:
		Motor_Direction(MOTOR_1,MOTOR_FORWARD);
		Motor_Direction(MOTOR_2,MOTOR_BACKWARD);
		Motor_Start(MOTOR_1,speed);
		Motor_Start(MOTOR_2,speed);
		break;
		
		case CAR_RIGHT:
		Motor_Direction(MOTOR_1,MOTOR_BACKWARD);
		Motor_Direction(MOTOR_2,MOTOR_FORWARD);
		Motor_Start(MOTOR_1,speed);
		Motor_Start(MOTOR_2,speed);
		break;
		
		case CAR_STOP:
		Motor_Direction(MOTOR_1,MOTOR_STOP);
		Motor_Direction(MOTOR_2,MOTOR_STOP);
		Motor_Start(MOTOR_1,speed);
		Motor_Start(MOTOR_2,speed);
		break;
		
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}