/*
 * DIO.c
 *
 * Created: 2/16/2020 4:22:20 PM
 *  Author: Reem
 */ 
#include "DIO.h"
/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info){
	if(DIO_info == NULL){
		return E_NOK;
	}
	else{
		switch(DIO_info->GPIO){
			case GPIOA:
			if(DIO_info->dir == OUTPUT){
				set_bit_mask(PORTA_DIR,DIO_info->pins);
			}
			else if(DIO_info->dir == INPUT){
				clr_bit_mask(PORTA_DIR,DIO_info->pins);
			}
			else{
				return E_NOK;
			}
			/*clr_bit_mask(PORTA_DIR,DIO_info->pins);
			PORTA_DIR = (DIO_info->dir & DIO_info->pins);*/
			break;
			
			case GPIOB:
			if(DIO_info->dir == OUTPUT){
				set_bit_mask(PORTB_DIR,DIO_info->pins);
			}
			else if(DIO_info->dir == INPUT){
				clr_bit_mask(PORTB_DIR,DIO_info->pins);
			}
			else{
				return E_NOK;
			}
			/*clr_bit_mask(PORTB_DIR,DIO_info->pins);
			PORTB_DIR = (DIO_info->dir & DIO_info->pins);*/
			break;
			
			case GPIOC:
			if(DIO_info->dir == OUTPUT){
				set_bit_mask(PORTC_DIR,DIO_info->pins);
			}
			else if(DIO_info->dir == INPUT){
				clr_bit_mask(PORTC_DIR,DIO_info->pins);
			}
			else{
				return E_NOK;
			}
			/*clr_bit_mask(PORTC_DIR,DIO_info->pins);
			PORTC_DIR = (DIO_info->dir & DIO_info->pins);*/
			break;
			
			case GPIOD:
			if(DIO_info->dir == OUTPUT){
				set_bit_mask(PORTD_DIR,DIO_info->pins);
			}
			else if(DIO_info->dir == INPUT){
				clr_bit_mask(PORTD_DIR,DIO_info->pins);
			}
			else{
				return E_NOK;
			}
			/*clr_bit_mask(PORTD_DIR,DIO_info->pins);
			PORTD_DIR = (DIO_info->dir & DIO_info->pins);*/
			break;
			
			default:
			return E_NOK;
			break;
		}
		return E_OK;
	}
	
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value){
	switch(GPIO){
		case GPIOA:
		if(value == HIGH){
			set_bit_mask(PORTA_DATA,pins);
		}
		else if(value == LOW){
			clr_bit_mask(PORTA_DATA,pins);
		}
		else{
			return E_NOK;
		}
		/*clr_bit_mask(PORTA_DATA,pins);
		PORTA_DATA |= (value & pins);*/
		break;
		
		case GPIOB:
		if(value == HIGH){
			set_bit_mask(PORTB_DATA,pins);
		}
		else if(value == LOW){
			clr_bit_mask(PORTB_DATA,pins);
		}
		else{
			return E_NOK;
		}
		/*clr_bit_mask(PORTB_DATA,pins);
		PORTB_DATA |= (value & pins);*/
		break;
		
		case GPIOC:
		if(value == HIGH){
			set_bit_mask(PORTC_DATA,pins);
		}
		else if(value == LOW){
			clr_bit_mask(PORTC_DATA,pins);
		}
		else{
			return E_NOK;
		}
		/*clr_bit_mask(PORTC_DATA,pins);
		PORTC_DATA |= (value & pins);*/
		break;
		
		case GPIOD:
		if(value == HIGH){
			set_bit_mask(PORTD_DATA,pins);
		}
		else if(value == LOW){
			clr_bit_mask(PORTD_DATA,pins);
		}
		else{
			return E_NOK;
		}
		/*clr_bit_mask(PORTD_DATA,pins);
		PORTD_DATA |= (value & pins);*/
		break;
		
		default:
		 return E_NOK;
		break;
	}
	return E_OK;
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data){
	if (data == NULL)
	{
		return E_NOK;
	}
	else{
		switch(GPIO){
			//reading the pin in portA
			case GPIOA:
			(*data) =get_bit(PORTA_PIN,pins);		
			break;
			
			//reading the pin in portB
			case GPIOB:
			(*data) =get_bit(PORTB_PIN,pins);
			break;
			
			//reading the pin in portC
			case GPIOC:
			(*data) =get_bit(PORTC_PIN,pins);	
			break;
			
			//reading the pin in portD
			case GPIOD:
			(*data) =get_bit(PORTD_PIN,pins);
			break;
			
			//Invalid GPIO Port
			default:
				return E_NOK;
			break;
		}
		
		return E_OK;
	}
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins){
	switch(GPIO){
		//toggle pins in port A
		case GPIOA:
		toggle_bit_mask(PORTA_DATA,pins);
		break;
		
		//toggle pins in port B
		case GPIOB:
		toggle_bit_mask(PORTB_DATA,pins);
		break;
		
		//toggle pins in port C
		case GPIOC:
		toggle_bit_mask(PORTC_DATA,pins);
		break;
		
		//toggle pins in port D
		case GPIOD:
		toggle_bit_mask(PORTD_DATA,pins);
		break;
		
		//Invalid port returning error
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}