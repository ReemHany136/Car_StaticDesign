/*
 * DEI.h
 *
 * Created: 2/18/2020 11:37:05 PM
 *  Author: Reem
 */ 


#ifndef DEI_H_
#define DEI_H_

/******************************************/
/*               Defines                  */
/******************************************/

#define DEI_INT0_PORT      GPIOD
#define DEI_INT0_PIN       BIT2

#define DEI_INT1_PORT      GPIOD
#define DEI_INT1_PIN       BIT3

#define DEI_INT2_PORT      GPIOB
#define DEI_INT2_PIN       BIT2


#define _DEI_INT0          0
#define _DEI_INT1          1
#define _DEI_INT2          2

/******************************************/
/*             FUNCTION PROTYPES          */
/******************************************/

/*
 * Fun----------: ERROR_STATUS DEI_Init(uint8_t DEI_Ch);
 * Input--------: digital external interrupt channel
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
*/




#endif /* DEI_H_ */