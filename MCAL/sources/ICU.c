/*
 * ICU.c
 *
 * Created: 2/16/2020 4:22:38 PM
 *  Author: Reem
 */ 
#include "ICU.h"
#include "interrupts.h"


static volatile uint8_t gu8_EdgeFlag = FALSE;
static volatile uint8_t gu8_ReadingFlag = FALSE;
static uint16_t gu16_no_of_counts=0;
static uint32_t gu32_no_of_ovf=0;
static volatile uint8_t gu8_no_of_ISR_Entrance=0;
static uint8_t gu8_ICU_ReadingMode;
static uint8_t gu8_ICU_TimeCH;

/**************************************************************************
*                   DEI ISR for measuring the time of the pulse           * 
**************************************************************************/

_ISR__(INT2_vect){
	switch(gu8_ICU_ReadingMode){
		case ICU_RISE_TO_RISE:
		break;
		
		case ICU_RISE_TO_FALL:
		switch(gu8_ICU_TimeCH){
			case ICU_TIMER_CH0:
				if (gu8_EdgeFlag == TRUE)
				{
					gu8_EdgeFlag=FALSE;
					// update the interrupt firing status to fire in falling edge 
					clr_bit(MCUCSR,MCUCSR_INT2_BIT);
					Timer_Start(ICU_TIMER_CH0,0);
				}
			
				else{
					Timer_Stop(ICU_TIMER_CH0);
					//Indication that measuring the time of the pulse is finished
 					gu8_no_of_ISR_Entrance=TRUE;	
					set_bit(MCUCSR,MCUCSR_INT2_BIT);
					
				
					Timer_GetValue(ICU_TIMER_CH0, &gu16_no_of_counts);
					Timer_GetNoOfOVFS(ICU_TIMER_CH0, &gu32_no_of_ovf);
					
				
				}
			
			break;
			
			case ICU_TIMER_CH1:
			break;
			
			case ICU_TIMER_CH2:
			break;
		}
		
		break;
		
		case ICU_FALL_TO_RISE:
		break;
		
		default:
		break;
	}
	
}

/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/
ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg){
	if (Icu_Cfg == NULL)
	{
		return E_NOK;
	}
	else{
		DIO_Cfg_s astr_dio_Cfg;
		Timer_cfg_s astr_timer_cfg;
		switch(Icu_Cfg->ICU_Ch_No){
			//DEI channel 0
			case ICU_CH0:
			break;
			
			//DEI channel 1
			case ICU_CH1:
			break;
			
			//DEI channel 2
			case ICU_CH2:
			
			
			astr_dio_Cfg.GPIO =DEI_ICU_PORT;
			astr_dio_Cfg.dir =INPUT;
			astr_dio_Cfg.pins=DEI_ICU_PIN;
			
			DIO_init(&astr_dio_Cfg);
		    
			switch(Icu_Cfg->ICU_Ch_Timer){
				//Timer channel 0
				case ICU_TIMER_CH0:
				
				astr_timer_cfg.Timer_CH_NO = ICU_TIMER_CH0;
				astr_timer_cfg.Timer_Mode =TIMER_MODE;
				astr_timer_cfg.Timer_Polling_Or_Interrupt =TIMER_POLLING_MODE;
				astr_timer_cfg.Timer_Prescaler= ICU_Prescale_No
				Timer_Init(&astr_timer_cfg);
				
				gu8_ICU_TimeCH = ICU_TIMER_CH0;
				
				
				break;
				
				//Timer channel 1
				case ICU_TIMER_CH1:
				break;
				
				//Timer channel 2
				case ICU_TIMER_CH2:
				break;
				
				//Invalid timer channel 
				default:
				return E_NOK;
				break;
			}
			break;
			
			//Invalid DEI Channel
			default:
			return E_NOK;
			break;
		}
	}
	return E_OK;
}

/***************************************************************************
 * Function		: Icu_ReadTime
 * Input		: 
 *				Icu_Channel --> ICU timer channel
 *									- ICU_TIMER_CH0
 *									- ICU_TIMER_CH1
 *									- ICU_TIMER_CH2
 *				Icu_EdgeToEdge -- > edges to calculate pusle time between:
 *									- ICU_RISE_TO_RISE
 *									- ICU_RISE_TO_FALL
 *									- ICU_FALE_TO_RISE					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from falling edge to rising edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between 2 edges				       *
 ***************************************************************************/
ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel, uint8_t Icu_EdgeToEdge, uint32_t * Icu_Time){
	
	float af_tickTimeInMicros;
	switch(Icu_Channel){
		
		//TIMER channel 0
		case ICU_CH0:
		
		switch(Icu_EdgeToEdge){
			case ICU_RISE_TO_RISE:
			break;
			
			
			case ICU_RISE_TO_FALL:
			
			//start with detecting the rising Edge by adjusting the peripheral interrupt to fire every rising edge
			
			gu8_ICU_ReadingMode = ICU_RISE_TO_FALL;
			gu8_EdgeFlag =TRUE;
			set_bit(GICR, GICR_INT2_BIT);
			set_bit(MCUCSR,MCUCSR_INT2_BIT);
			EN_GLOBAL_INT;
			//wait until measuring the whole period of the pulse and that's by entering the ISR twice for both rising and falling edge
			
			while(gu8_no_of_ISR_Entrance != TRUE);
			gu8_no_of_ISR_Entrance=FALSE;
			af_tickTimeInMicros =(ICU_Prescale/FCPU_HZ);
			//(*Icu_Time) =  (gu16_no_of_counts*tickTimeInMicros)+(gu32_no_of_ovf*_8BIT_RESOLUTION_*tickTimeInMicros);
			(*Icu_Time) =  (gu16_no_of_counts*af_tickTimeInMicros);
			
			break;
			
			case ICU_FALL_TO_RISE:
			break;
			
			default:
			return E_NOK;
		}
		
		break;
		
		//TIMER channel 1
		case ICU_CH1:
		break;
		
		//TIMER channel 2
		case ICU_CH2:
		break;
		
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}

