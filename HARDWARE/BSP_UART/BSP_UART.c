/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_UART.c
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_UART.h"	
#include "includes.h"

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

uint8_t   DMA_UART8_Rx_Buffer[UART8_Rx_Buffer_Num];  // DMA��UART8����ԭʼ���ݻ�����
uint8_t   UART8_Rx_Buffer[UART8_Rx_Buffer_Num];  // UART8����ԭʼ���ݻ�����
uint8_t   UART8_Rx_SIGN=0;					// UART8���������ݱ�־
uint8_t   UART8_Rx_Num=0;					//	UART8�������ݸ���

uint8_t   DMA_UART8_Tx_Buffer[UART8_Tx_Buffer_Num];  // DMA��UART8����ԭʼ���ݻ�����
uint8_t   UART8_Tx_Buffer[UART8_Tx_Buffer_Num];  // UART8����ԭʼ���ݻ�����
uint8_t   UART8_Tx_SIGN=0;					// UART8���ڷ��������ݱ�־

DMA_HandleTypeDef  	UART8TxDMA_Handler,UART8RxDMA_Handler; //DMA���
UART_HandleTypeDef 	UART8_Handler; //UART���
	
/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/

//��ʼ��IO ����1 
//bound:������
void UART8_Init(u32 bound)
{ 
		UART8_Handler.Instance=UART8;					    //USART8
		UART8_Handler.Init.BaudRate=bound;				    //������
		UART8_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
		UART8_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
		UART8_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
		UART8_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
		UART8_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	  UART8_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
		UART8_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART8_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		HAL_UART_Init(&UART8_Handler);					    //HAL_UART_Init()��ʹ��UART8
} 


//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж����ã�DMA����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��
//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
		GPIO_InitTypeDef 	GPIO_Initure;//GPIO���
		
		__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOEʱ��
		__HAL_RCC_UART8_CLK_ENABLE();			//ʹ��UART8ʱ��
		__HAL_RCC_DMA1_CLK_ENABLE();				//DMA1ʱ��ʹ��

		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;			//PE0,1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART8;	//����ΪUART8
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//��ʼ��PE0,1
	
	
		/*********************OUT************************/
		GPIO_Initure.Pin=GPIO_PIN_2; //
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);	//RS485������ģʽ
		
	
		//Tx DMA����
		UART8TxDMA_Handler.Instance=DMA1_Stream0;                           //������ѡ��
		UART8TxDMA_Handler.Init.Channel=DMA_CHANNEL_5;                      //ͨ��ѡ��
		UART8TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������
		UART8TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
		UART8TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
		UART8TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
		UART8TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
		UART8TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
		UART8TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
		UART8TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
		UART8TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
		UART8TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
		UART8TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
		
		HAL_DMA_DeInit(&UART8TxDMA_Handler);   //��DMA�Ĵ�������Ϊȱʡֵ
		HAL_DMA_Init(&UART8TxDMA_Handler);		//��ʼ��DMA
		__HAL_LINKDMA(huart,hdmatx,UART8TxDMA_Handler);    //��DMA��USART8��ϵ����(����DMA)
		
		//Rx DMA����
		UART8RxDMA_Handler.Instance=DMA1_Stream6;                           //������ѡ��
		UART8RxDMA_Handler.Init.Channel=DMA_CHANNEL_5;                      //ͨ��ѡ��
		UART8RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //���赽�洢��
		UART8RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
		UART8RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
		UART8RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
		UART8RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
		UART8RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //��������ģʽ
		UART8RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
		UART8RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
		UART8RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
		UART8RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
		UART8RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
		
		HAL_DMA_DeInit(&UART8RxDMA_Handler);   	//��DMA�Ĵ�������Ϊȱʡֵ
		HAL_DMA_Init(&UART8RxDMA_Handler);			//��ʼ��DMA
		__HAL_LINKDMA(huart,hdmarx,UART8RxDMA_Handler);    //��DMA��UART8��ϵ����(����DMA)
		
		__HAL_UART_CLEAR_IDLEFLAG(&UART8_Handler);//��������ж�
		__HAL_UART_CLEAR_IT(&UART8_Handler, UART_CLEAR_TCF);//�����������ж�
		
		__HAL_UART_ENABLE_IT(&UART8_Handler,UART_IT_IDLE);		//�������н����ж�
		__HAL_UART_ENABLE_IT(&UART8_Handler,UART_IT_TC);			//������������ж�
		
		__HAL_UART_CLEAR_IDLEFLAG(&UART8_Handler);//��������ж�
		__HAL_UART_CLEAR_IT(&UART8_Handler, UART_CLEAR_TCF);//�����������ж�	
	
		HAL_UART_Receive_DMA(&UART8_Handler,(uint8_t*)DMA_UART8_Rx_Buffer,UART8_Rx_Buffer_Num);//����DMAͨ������������
		
		__HAL_UART_DISABLE_IT(&UART8_Handler,UART_IT_ERR);			//
		__HAL_UART_DISABLE_IT(&UART8_Handler,UART_IT_PE);			//
	
		
		HAL_NVIC_SetPriority(UART8_IRQn,2,1);			//��ռ���ȼ�2�������ȼ�1������TIMʱ��
		HAL_NVIC_EnableIRQ(UART8_IRQn);				//ʹ��UART8�ж�ͨ��
		
		/* DMA1_Stream0_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 2, 1);//��ռ���ȼ�2�������ȼ�1������TIMʱ��
		HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
		/* DMA1_Stream6_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 2, 1);//��ռ���ȼ�2�������ȼ�1������TIMʱ��
		HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}



/*
�������ܣ�����8DMA���ݷ���
�����βΣ�Sendbuff ����������
          Bufflens �����ݳ���
��������ֵ�����ݳ���
��ע����
*/
void UART8_DMA_Sent(u8 * Sendbuff, u16 Bufflens)
{		
		u8 i=0;
		if(UART8_Tx_SIGN==0)
		{
				u16 l_val = Bufflens > UART8_Tx_Buffer_Num ? UART8_Tx_Buffer_Num : Bufflens;
				if(l_val > 0)
				{
						for(i=0;i<5;i++)
						{
							RS485_Tx_Mode;
						}
						memcpy(DMA_UART8_Tx_Buffer, Sendbuff, l_val);
						SCB_InvalidateDCache_by_Addr((u32*)DMA_UART8_Tx_Buffer,l_val);
						HAL_UART_Transmit_DMA(&UART8_Handler, DMA_UART8_Tx_Buffer, l_val);
						UART8_Tx_SIGN=1;
				}
		}
}


void UART8_IRQHandler(void)
{
		HAL_UART_IRQHandler(&UART8_Handler);
	
		//��������ж�
		if(__HAL_UART_GET_FLAG(&UART8_Handler, UART_FLAG_IDLE) != RESET)
		{
			u16 l_val=0;
			u32 temp=0;
			__HAL_UART_CLEAR_IDLEFLAG(&UART8_Handler);//��������ж�
			temp=UART8_Handler.Instance->ISR;
			temp=UART8_Handler.Instance->RDR;
			/*ʧ��DMA����*/
			HAL_UART_DMAStop(&UART8_Handler);
			SCB_InvalidateDCache_by_Addr((u32*)DMA_UART8_Rx_Buffer,UART8_Rx_Buffer_Num);
			l_val = UART8_Rx_Buffer_Num - UART8_Handler.hdmarx->Instance->NDTR;  // ͨ��DMA����ָ�������յ��ֽ���
			if(l_val!=0)
			{
				/*���ձ�־λ��1*/
				//UART8_Rx_SIGN=1;	
				WirteRingbuffer_UART8Rx(DMA_UART8_Rx_Buffer,l_val);	
				//memcpy(UART2_Rx_Buffer,DMA_UART2_Rx_Buffer,UART2_Rx_Num);
				//memset(DMA_UART2_Rx_Buffer,0,sizeof(DMA_UART2_Rx_Buffer));
				HAL_UART_Receive_DMA(&UART8_Handler, DMA_UART8_Rx_Buffer, UART8_Rx_Buffer_Num); //����DMA����
				__HAL_UART_DISABLE_IT(&UART8_Handler, UART_IT_ERR);
				__HAL_UART_DISABLE_IT(&UART8_Handler, UART_IT_PE);
				
			}
			else
			{
				READ_REG(UART8_Handler.Instance->RDR);
				__HAL_UART_CLEAR_OREFLAG(&UART8_Handler);
			}
		}
		
		
		//��������ж�
		if(__HAL_UART_GET_FLAG(&UART8_Handler, UART_FLAG_TC) != RESET)
		{
			__HAL_UART_CLEAR_IT(&UART8_Handler, UART_CLEAR_TCF);
			UART8_Tx_SIGN=0;
			RS485_Rx_Mode;
		} 
		/* USER CODE END USART1_IRQn 0 */
		/* USER CODE BEGIN USART1_IRQn 1 */

		/* USER CODE END USART1_IRQn 1 */
}



/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&UART8TxDMA_Handler);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&UART8RxDMA_Handler);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}
 
 
