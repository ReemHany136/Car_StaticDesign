#ifndef TESTER_H_
#define TESTER_H_

#include "Steering.h"
#define OK 0
#define NOK 1
#define TEST_CASES 5

void TESTING_SteeringMain(void);// Just call this function
unsigned char TESTING__Steering_Init(void);
unsigned char TESTING__Steering_SteerCar(unsigned char direction,unsigned char speed);

#endif //#ifndef TESTER_H_
