/*
 * BCM_Project.c
 *
 * Created: 3/2/2020 7:08:35 PM
 *  Author: Reem
 */ 



#include "BCM.h"
#include "DIO.h"

/*
void Consumer(uint8_t Status);
uint8_t Data[30];
int main(void)
{
	strBCMCfg_t bcm;
	DIO_Cfg_s diocfg;
	diocfg.GPIO =GPIOB;
	diocfg.pins=BIT0|BIT1|BIT3;
	diocfg.dir=OUTPUT;
	
	DIO_init(&diocfg);
	
	bcm.Protocol = SPI_BCM;
	bcm.Direction =READING;
	BCM_Init(&bcm);
	UART_INIT(Uart_Parity_no,Uart_Stop_Bit_One,Uart_Data_8_Bit,Uart_Baud_Rate_9600,Uart_Async_Normal_Speed,Uart_Multi_Processor_Communication_Disable,Uart_Polling,NULL);
	
	BCM_SetupRxBuffer(Data,80,Consumer);
	
	while(1)
    {
        //TODO:: Please write your application code
		BCM_RxDispatcher(); 
    }
	
}
void Consumer(uint8_t Status){
	DIO_Write(GPIOB,BIT3,OUTPUT);
	if(Status == E_OK){
		TCNT0 = Data[9];

		UART_TRANSMIT_String(Data);
		BCM_RxUnlock(RX_UNLOCK);
	}
}
*/	




extern  strTxBuffer_t Tx_RequestBuffer[];
int main  (void)
{
	strBCMCfg_t BCM_Info;
	DIO_Cfg_s diocfg;
	uint8_t Data[100];
	
	BCM_Info.Direction = SENDING;
	BCM_Info.Protocol = SPI_BCM;
	
		
	diocfg.GPIO =GPIOB;
	diocfg.pins=BIT0|BIT1|BIT3;
	diocfg.dir=OUTPUT;
		
	DIO_init(&diocfg);
	
	
	UART_INIT(Uart_Parity_no,Uart_Stop_Bit_One,Uart_Data_8_Bit,Uart_Baud_Rate_9600,Uart_Async_Normal_Speed,Uart_Multi_Processor_Communication_Disable,Uart_Polling,NULL);
	DIO_Write(GPIOA,BIT3,HIGH);
	
	UART_RECEIVE_String(Data);
	
	
	
	uint8_t D[6]={'1','2','3','4','5','6'};
	BCM_Init(&BCM_Info);
	Tx_RequestBuffer[0].Lock = TX_BUFFER_READY_TO_SEND;
	Tx_RequestBuffer[0].ptrTxBuffer = Data;
	Tx_RequestBuffer[0].Size = 9;
DIO_Write(GPIOB,BIT3,HIGH);
	BCM_Send(&Tx_RequestBuffer[0]);
	while (1)
	{
	    
	  
		
		BCM_TxDispatcher();
	}

	return 0;
}