/*
 * BCM.c
 *
 * Created: 3/2/2020 7:17:30 PM
 *  Author: Reem
 */ 

#include "BCM.h"

/***************/
/***** LOCAL MACROS *****/
/***************/

#define NOT_INITIALIZED 0
#define INITIALIZED 0xFF

#define TX_CHECK_SUM_INITIAL_VALUE 0
#define CREATE_REQUEST_BUFFER_COUNTER_INITAIL_VALUE 0
#define NUMBER_OF_REQUEST_FOR_TX_BCM 1
#define IDLE_COUNTER_INITIAL_VALUE 0



/*Tx frame bytes number*/
#define TX_FRAME_COUNTER_INITAIL_VALUE 0
#define ID_BYTE 1
#define SIZE_FIRST_BYTE 2
#define SIZE_SECOND_BYTE 3
#define FIRST_DATA_BYTE_IN_FRAME 4 

/*TX DIS states*/
#define IDLE 1
#define SENDING_BYTE 2
#define SEND_BYTE_COMPLETE 3
#define FRAME_COMPLETE 4

/***************/
/***** LOCAL STRUCT *****/
/***************/




/*****************/
/***** GLOBAL VARIABLES *****/
/***************/

static uint8_t gu8_Protocol = NOT_INITIALIZED;
static void (*ptrSend) (uint8_t,uint8_t*) = NULL;
/* strTxBuffer_t gstrTxBuffer;*/
static uint8_t gu8_TxCreatorFlag = NOT_INITIALIZED;
static uint8_t gu8_TxCheckSum = TX_CHECK_SUM_INITIAL_VALUE;

static uint8_t gu8_TxByteSendFlag = NOT_INITIALIZED;
strTxBuffer_t Tx_RequestBuffer [NUMBER_OF_REQUEST_FOR_TX_BCM];

/*Local defines*/
#define BCM_ID                    'A'
#define RX_BCMID_BYTE_NUM          0
#define RX_SIZE_LSB_BYTE_NUM       1
#define RX_SIZE_MSB_BYTE_NUM       2
#define RX_DATASTART_BYTE_NUM      3

#define RX_BUFFER_REMAINING_SIZE   3

static strRecievedInfo_t  gstr_RxInfo;

static uint8_t gu8_Protocol , gu8_CurrentState , gu8_Direction,
			   gu8_FrameRecieveFlag , gu8_RXBufferStateLock ,
			   gu8_SizeErrorFlag ;
			   
static uint16_t gu16_CurrentFrameCounter , gu16_SizeMSB, gu16_SizeLSB;

static strBufferInfo_t gstr_RxBuffer;

static void (*RxBCM_CBK)(uint8_t);

/*Static Prototypes*/
void BCM_ISR_CBK(uint8_t RecievedByte);

/*Function implementation*/

void BCM_ISR_CBK(uint8_t RecievedByte){
	
	if ((gstr_RxInfo.RxCounter== RX_BCMID_BYTE_NUM) && (RecievedByte == BCM_ID) && (gu8_RXBufferStateLock == RX_UNLOCK))
	{
		gu8_FrameRecieveFlag = TRUE;
		gu8_RXBufferStateLock = RX_RECIEVING;
		
		gstr_RxInfo.RxCounter++;
	}
	
	else if((gu8_RXBufferStateLock == RX_RECIEVING)){
		
		if(gstr_RxInfo.RxCounter == RX_SIZE_LSB_BYTE_NUM){
			gu16_SizeLSB = RecievedByte;
		}
		else if(gstr_RxInfo.RxCounter == RX_SIZE_MSB_BYTE_NUM){
			//e3mly el size
			gu16_SizeMSB = RecievedByte;
			gstr_RxInfo.RxDataSize = (gu16_SizeMSB << 8) |gu16_SizeLSB;
		}
		
		
		else if((gstr_RxInfo.RxCounter > RX_SIZE_MSB_BYTE_NUM) && (gstr_RxInfo.RxCounter < (gstr_RxInfo.RxDataSize+RX_BUFFER_REMAINING_SIZE))){//DIO_Write(GPIOA,BIT0,HIGH);
			
			
			gstr_RxBuffer.RxBuffer[gstr_RxBuffer.RxBufferIndex] = RecievedByte;
			
			
			gstr_RxInfo.CheckSum += gstr_RxBuffer.RxBuffer[gstr_RxBuffer.RxBufferIndex];
			
			
			gstr_RxBuffer.RxBufferIndex++;
		}
		
		
		else if((gstr_RxInfo.RxCounter == (gstr_RxInfo.RxDataSize+RX_BUFFER_REMAINING_SIZE))){
			//TCNT2 =gstr_RxInfo.RxCounter;
			gstr_RxInfo.CheckSumRecieved = RecievedByte;
		}
		
		
		gstr_RxInfo.RxCounter++;
	}
	
}

ERROR_STATUS BCM_Init(strBCMCfg_t *BCMCfg){
	ERROR_STATUS ERR;
	if (BCMCfg != NULL)
	{
		switch(BCMCfg->Protocol){
			case UART_BCM:
			gu8_Protocol = UART_BCM;
			UART_INIT(Uart_Parity_no,Uart_Stop_Bit_One,Uart_Data_8_Bit,Uart_Baud_Rate_9600,Uart_Async_Normal_Speed,Uart_Multi_Processor_Communication_Disable,Uart_Interrupt,BCM_ISR_CBK);
			ERR =E_OK;
			break;
			
			case SPI_BCM:
			gu8_Protocol = SPI_BCM;
			if(BCMCfg->Direction == SENDING){
				_SPIInitMaster(Fosc4 , mode1 ,MSB,SPI_INTERRUPT_MODE);
				
			}
			else if (BCMCfg->Direction == READING)
			{
				DIO_Write(GPIOA,BIT1,HIGH);
				_SPIInitSlave(Fosc4 , mode1 ,MSB,SPI_INTERRUPT_MODE,BCM_ISR_CBK);
			}
			ERR =E_OK;
			break;
			
			default:
			ERR =(BCM_ERROR + E_INVALID_PARAMETER);
			break;
			
		}
		if(ERR == E_OK){
			/* Initializing Globals and check on the direction*/
			
			switch(BCMCfg->Direction){
				case READING:
					
					gu8_Direction = READING;
					
					gu8_RXBufferStateLock = RX_UNLOCK;
					gu8_CurrentState = RX_IDLE;
					
					gu16_SizeMSB =0;
					gu16_SizeLSB =0;
					
					gu16_CurrentFrameCounter=0;
					
					
					RxBCM_CBK = NULL;
					
					gstr_RxInfo.RxCounter =0;
					gstr_RxInfo.CheckSum=0;
					
					gstr_RxBuffer.RxBuffer = NULL;
					gstr_RxBuffer.RxBufferIndex=0;
					gstr_RxBuffer.RxBufferSize=0;
					
					gu8_SizeErrorFlag = FALSE;
					
					ERR =E_OK;
				break;
				
				case SENDING:
					/* make send call pointer point to protocol send function*/
					if (UART_BCM == gu8_Protocol)
					{
						ptrSend = UART_TRANSMIT_Char;
					}

					else if (SPI_BCM == gu8_Protocol)
					{
						ptrSend = _SPITrancevier; //ptrSend = spi transmit function
						
					}
					
					//should move to inti function
					uint8_t au8_CreatRequestBufferCounter = CREATE_REQUEST_BUFFER_COUNTER_INITAIL_VALUE;
					
					/* create Tx request buffer*/
					for (au8_CreatRequestBufferCounter = CREATE_REQUEST_BUFFER_COUNTER_INITAIL_VALUE; au8_CreatRequestBufferCounter < NUMBER_OF_REQUEST_FOR_TX_BCM; au8_CreatRequestBufferCounter++)
					{
						Tx_RequestBuffer[au8_CreatRequestBufferCounter].Lock = TX_BUFFER_UNLOCK;
						Tx_RequestBuffer[au8_CreatRequestBufferCounter].ptrTxBuffer = NULL;
						Tx_RequestBuffer[au8_CreatRequestBufferCounter].Size = ZERO;
					}
				
				break;
				
				default:
				ERR =(BCM_ERROR + E_INVALID_PARAMETER);
				break; 
			}
			
		}
		
	}
	else{
		ERR =(BCM_ERROR + E_NULL_PTR);
	}
	return ERR;
}

ERROR_STATUS BCM_SetupRxBuffer(uint8_t* PtrRxData,uint16_t size,void (*Notification)(uint8_t)){
	ERROR_STATUS ERR;
	if(PtrRxData != NULL && Notification != NULL){
		RxBCM_CBK = Notification;
		gstr_RxBuffer.RxBuffer = PtrRxData;
		gstr_RxBuffer.RxBufferSize =size;
		ERR = E_OK;
	}
	else{
		ERR = (BCM_ERROR + E_NULL_PTR);
	}
	return ERR;
	
}

ERROR_STATUS BCM_RxUnlock(uint8_t Rxlock){
	ERROR_STATUS ERR;
	
	gu8_RXBufferStateLock = Rxlock;
	gstr_RxBuffer.RxBufferIndex=0;
	
	ERR = E_OK;
	return ERR;
}

/*
ERROR_STATUS BCM_GetRxBuffer(uint8_t * BufferState){
	
}*/

void BCM_RxDispatcher(void){
	
	switch(gu8_CurrentState){
		
		case RX_IDLE:
		if (gu8_RXBufferStateLock == RX_RECIEVING)
		{
			gu8_CurrentState = RX_RECIEVING_BYTE;
		}
		
		
		break;
		
		case  RX_RECIEVING_BYTE:
		/*Loop on what is came from the ISR and make the check on this data*/
		while(gu16_CurrentFrameCounter < gstr_RxInfo.RxCounter){
			
			/*Check whether the size is received fully with success or not*/
			if(gu16_CurrentFrameCounter == RX_DATASTART_BYTE_NUM){
				
				/*Compare the Received Size  and the Buffer Size*/
				if(gstr_RxBuffer.RxBufferSize <gstr_RxInfo.RxDataSize){
					/*Notify error and send it to the call back function Or make an error state*/
					gu8_SizeErrorFlag = FALSE;
				}
				else{
					gu8_SizeErrorFlag = TRUE;
				}
				
			}
			
			/*Check whether the current received byte is a data or not to move to the next state*/
			if(gu16_CurrentFrameCounter == (gstr_RxInfo.RxDataSize+RX_BUFFER_REMAINING_SIZE)){
				//Switch to the received complete frame state
				
				gu8_CurrentState = RX_RECIEVING_COMPLETE_FRAME;
				break;
			}
			
			//Increment the counter until it reaches the ISR Received Counter/
			gu16_CurrentFrameCounter++;
		}
		
		
		break;
		
		case  RX_RECIEVING_COMPLETE_FRAME:
		if((gstr_RxInfo.CheckSum == gstr_RxInfo.CheckSumRecieved) && (gu8_SizeErrorFlag == TRUE)){
			
			//Callback the consumer send it E_OK
			
			RxBCM_CBK(E_OK);
			gu8_RXBufferStateLock =RX_READY;
		}
		else{
			//Callback the consumer send it E_NOK
			RxBCM_CBK(E_NOK);
			gu8_RXBufferStateLock =RX_UNLOCK;
		}
		/*RxBCM_CBK(E_OK);
		gu8_RXBufferState =RX_READY;*/
		gu8_CurrentState = RX_IDLE;
		break;
		
	}
}


ERROR_STATUS BCM_Send(strTxBuffer_t * TxRequest)
{
	uint8_t au8_Error = E_OK;
	
	
	/* check the pointer*/
	if (NULL != TxRequest->ptrTxBuffer)
	{
		/* check data size*/
		if (ZERO != TxRequest->Size)
		{
			/* check tx buffer lock state*/
			if(TX_BUFFER_READY_TO_SEND == Tx_RequestBuffer->Lock)
			{
				/* confirm that request is valid*/
				TxRequest->Lock = TX_BUFFER_SEND_VALID;
			}

			else{}
			//Error_Push(BCM_MODULE, ERROR_TX_BUFFER_NOT_READY);
		}
		else
		{
			//Error_Push(BCM_MODULE, ERROR_NO_DATA);
		}
	}
	else
	{
		//Error_Push(BCM_MODULE,ERROR_NULL_POINTER);
	}

	/* initialize global creator flag to let dis know there is creator to switch it's state from idle to sending byte */
	gu8_TxCreatorFlag = INITIALIZED;

	/* initialize the check by initial value */
	gu8_TxCheckSum = TX_CHECK_SUM_INITIAL_VALUE;

	return au8_Error;
}

void BCM_TxDispatcher (void)
{
	static uint8_t au8_TxDisState = IDLE;
	static uint8_t au8_TxFrameCompleteFlag = NOT_INITIALIZED;
	static uint8_t au8_IdleCounter = IDLE_COUNTER_INITIAL_VALUE;
	static uint16_t gu16_TxFrameCounter = TX_FRAME_COUNTER_INITAIL_VALUE;
	
	switch (au8_TxDisState)
	{
		case IDLE:
		//adjust the idle state according to the edit in sending due to array of request
		/* check if app call the creator */
		for (au8_IdleCounter=IDLE_COUNTER_INITIAL_VALUE;au8_IdleCounter<NUMBER_OF_REQUEST_FOR_TX_BCM;au8_IdleCounter++)
		{
			if (TX_BUFFER_SEND_VALID == Tx_RequestBuffer[au8_IdleCounter].Lock)
			{
				/* lock the Tx buffer to prevent the app from using the buffer during sending the data */
				Tx_RequestBuffer[au8_IdleCounter].Lock = TX_BUFFER_SENDING;

				/* Change the Tx DIS state to Sending byte to start send the frame */
				au8_TxDisState = SENDING_BYTE;

				/* Save the requested id*/

				gu16_TxFrameCounter++;
				break;
			}
			/* Do nothing */
			else
			;
		}
		
		break;

		/*********************************************************************************************************************************/
		case SENDING_BYTE:
		/* check if it the ID byte (first byte in the frame) */
		if (ID_BYTE == gu16_TxFrameCounter)
		{
			/* send the BCM ID byte */
			ptrSend('A', &gu8_TxByteSendFlag);//change the sending data by BCM ID variable
		}
		
		/* check if it the size first byte (second byte in the frame) */
		else if ( SIZE_FIRST_BYTE == gu16_TxFrameCounter)
		{
			/* send the frame size first byte */
			ptrSend(Tx_RequestBuffer[au8_IdleCounter].Size, &gu8_TxByteSendFlag);//change the sending data by first size variable
		}

		/* check if it the size second byte (third byte in the frame) */
		else if ( SIZE_SECOND_BYTE == gu16_TxFrameCounter)
		{
			/* send the frame size second byte */
			ptrSend(((Tx_RequestBuffer[au8_IdleCounter].Size)>>8), &gu8_TxByteSendFlag);//change the sending data by second size variable
		}

		/* check if it the data byte */
		else if ((FIRST_DATA_BYTE_IN_FRAME == gu16_TxFrameCounter) || ((Tx_RequestBuffer[au8_IdleCounter].Size + FIRST_DATA_BYTE_IN_FRAME) > gu16_TxFrameCounter))
		{
			/* send the data byte */
			ptrSend(Tx_RequestBuffer[au8_IdleCounter].ptrTxBuffer[gu16_TxFrameCounter - FIRST_DATA_BYTE_IN_FRAME], &gu8_TxByteSendFlag);

			/* Calculate the Tx check sum*/
			gu8_TxCheckSum = gu8_TxCheckSum + Tx_RequestBuffer[au8_IdleCounter].ptrTxBuffer[gu16_TxFrameCounter - FIRST_DATA_BYTE_IN_FRAME];
		}

		else if ((Tx_RequestBuffer[au8_IdleCounter].Size + FIRST_DATA_BYTE_IN_FRAME) == gu16_TxFrameCounter)
		{
			/* send the Tx check sum byte*/
			ptrSend(gu8_TxCheckSum, &gu8_TxByteSendFlag);
			au8_TxFrameCompleteFlag = INITIALIZED;
		}

		au8_TxDisState = SEND_BYTE_COMPLETE;
		break;

		/********************************************************************************************************************************/
		case SEND_BYTE_COMPLETE:
		
		/* check if the current byte sent */
		if (INITIALIZED == gu8_TxByteSendFlag)
		{
			/* check if the sent byte was the last one */
			if (INITIALIZED == au8_TxFrameCompleteFlag)
			{
				/* change the DIS state to frame complete*/
				au8_TxDisState = FRAME_COMPLETE;
			}
			else
			{
				/* Increment the frame counter to send the next byte*/
				gu16_TxFrameCounter ++;
				
				/* return DIS state to sending byte to send the next byte*/
				au8_TxDisState = SENDING_BYTE;
			}
		}
		else
		{
			;/*stay in the same state for other iteration*/
		}
		break;
		
		/*****************************************/
		case FRAME_COMPLETE:
		/* unlock Tx buffer to provide the app to use the buffer*/
		Tx_RequestBuffer[au8_IdleCounter].Lock = TX_BUFFER_UNLOCK;
		Tx_RequestBuffer[au8_IdleCounter].ptrTxBuffer = NULL;
		Tx_RequestBuffer[au8_IdleCounter].Size = ZERO;
		
		/* notify the app that buffer send complete*/
		//still don't know how?? is it by call function or by set flag for the app i don't know till now
		
		/* return the Tx DIS to idle state*/
		au8_TxDisState = IDLE ;
		break;

		default:
		/* DO NOTHING*/
		break;
		
	}
}

ERROR_STATUS BCM_GetTxBuffer(strTxBuffer_t * TxRequestID,uint8_t * TxBufferState)
{
	uint8_t au8_Error = E_OK;
	
	*TxBufferState = TxRequestID->Lock;

	return au8_Error;
}