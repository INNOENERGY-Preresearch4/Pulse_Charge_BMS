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

uint8_t   DMA_UART8_Rx_Buffer[UART8_Rx_Buffer_Num];  // DMA中UART8接收原始数据缓冲区
uint8_t   UART8_Rx_Buffer[UART8_Rx_Buffer_Num];  // UART8接收原始数据缓冲区
uint8_t   UART8_Rx_SIGN=0;					// UART8接收新数据标志
uint8_t   UART8_Rx_Num=0;					//	UART8接收数据个数

uint8_t   DMA_UART8_Tx_Buffer[UART8_Tx_Buffer_Num];  // DMA中UART8发送原始数据缓冲区
uint8_t   UART8_Tx_Buffer[UART8_Tx_Buffer_Num];  // UART8发送原始数据缓冲区
uint8_t   UART8_Tx_SIGN=0;					// UART8正在发送新数据标志

DMA_HandleTypeDef  	UART8TxDMA_Handler,UART8RxDMA_Handler; //DMA句柄
UART_HandleTypeDef 	UART8_Handler; //UART句柄
	
/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/

//初始化IO 串口1 
//bound:波特率
void UART8_Init(u32 bound)
{ 
		UART8_Handler.Instance=UART8;					    //USART8
		UART8_Handler.Init.BaudRate=bound;				    //波特率
		UART8_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
		UART8_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
		UART8_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
		UART8_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
		UART8_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	  UART8_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
		UART8_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART8_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		HAL_UART_Init(&UART8_Handler);					    //HAL_UART_Init()会使能UART8
} 


//UART底层初始化，时钟使能，引脚配置，中断配置，DMA配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
		GPIO_InitTypeDef 	GPIO_Initure;//GPIO句柄
		
		__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
		__HAL_RCC_UART8_CLK_ENABLE();			//使能UART8时钟
		__HAL_RCC_DMA1_CLK_ENABLE();				//DMA1时钟使能

		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;			//PE0,1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF8_UART8;	//复用为UART8
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//初始化PE0,1
	
	
		/*********************OUT************************/
		GPIO_Initure.Pin=GPIO_PIN_2; //
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);	//RS485处于收模式
		
	
		//Tx DMA配置
		UART8TxDMA_Handler.Instance=DMA1_Stream0;                           //数据流选择
		UART8TxDMA_Handler.Init.Channel=DMA_CHANNEL_5;                      //通道选择
		UART8TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
		UART8TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
		UART8TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
		UART8TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
		UART8TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
		UART8TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
		UART8TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
		UART8TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
		UART8TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
		UART8TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
		UART8TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
		
		HAL_DMA_DeInit(&UART8TxDMA_Handler);   //把DMA寄存器设置为缺省值
		HAL_DMA_Init(&UART8TxDMA_Handler);		//初始化DMA
		__HAL_LINKDMA(huart,hdmatx,UART8TxDMA_Handler);    //将DMA与USART8联系起来(发送DMA)
		
		//Rx DMA配置
		UART8RxDMA_Handler.Instance=DMA1_Stream6;                           //数据流选择
		UART8RxDMA_Handler.Init.Channel=DMA_CHANNEL_5;                      //通道选择
		UART8RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //外设到存储器
		UART8RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
		UART8RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
		UART8RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
		UART8RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
		UART8RxDMA_Handler.Init.Mode=DMA_NORMAL;                            //外设流控模式
		UART8RxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
		UART8RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
		UART8RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
		UART8RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
		UART8RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
		
		HAL_DMA_DeInit(&UART8RxDMA_Handler);   	//把DMA寄存器设置为缺省值
		HAL_DMA_Init(&UART8RxDMA_Handler);			//初始化DMA
		__HAL_LINKDMA(huart,hdmarx,UART8RxDMA_Handler);    //将DMA与UART8联系起来(接收DMA)
		
		__HAL_UART_CLEAR_IDLEFLAG(&UART8_Handler);//清除空闲中断
		__HAL_UART_CLEAR_IT(&UART8_Handler, UART_CLEAR_TCF);//清除发送完成中断
		
		__HAL_UART_ENABLE_IT(&UART8_Handler,UART_IT_IDLE);		//开启空闲接收中断
		__HAL_UART_ENABLE_IT(&UART8_Handler,UART_IT_TC);			//开启发送完成中断
		
		__HAL_UART_CLEAR_IDLEFLAG(&UART8_Handler);//清除空闲中断
		__HAL_UART_CLEAR_IT(&UART8_Handler, UART_CLEAR_TCF);//清除发送完成中断	
	
		HAL_UART_Receive_DMA(&UART8_Handler,(uint8_t*)DMA_UART8_Rx_Buffer,UART8_Rx_Buffer_Num);//接收DMA通道关联缓存区
		
		__HAL_UART_DISABLE_IT(&UART8_Handler,UART_IT_ERR);			//
		__HAL_UART_DISABLE_IT(&UART8_Handler,UART_IT_PE);			//
	
		
		HAL_NVIC_SetPriority(UART8_IRQn,2,1);			//抢占优先级2，子优先级1，低于TIM时钟
		HAL_NVIC_EnableIRQ(UART8_IRQn);				//使能UART8中断通道
		
		/* DMA1_Stream0_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 2, 1);//抢占优先级2，子优先级1，低于TIM时钟
		HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
		/* DMA1_Stream6_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 2, 1);//抢占优先级2，子优先级1，低于TIM时钟
		HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}



/*
函数功能：串口8DMA数据发送
函数形参：Sendbuff ：缓冲数据
          Bufflens ：数据长度
函数返回值：数据长度
备注：无
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
	
		//接收完成中断
		if(__HAL_UART_GET_FLAG(&UART8_Handler, UART_FLAG_IDLE) != RESET)
		{
			u16 l_val=0;
			u32 temp=0;
			__HAL_UART_CLEAR_IDLEFLAG(&UART8_Handler);//清除空闲中断
			temp=UART8_Handler.Instance->ISR;
			temp=UART8_Handler.Instance->RDR;
			/*失能DMA接收*/
			HAL_UART_DMAStop(&UART8_Handler);
			SCB_InvalidateDCache_by_Addr((u32*)DMA_UART8_Rx_Buffer,UART8_Rx_Buffer_Num);
			l_val = UART8_Rx_Buffer_Num - UART8_Handler.hdmarx->Instance->NDTR;  // 通过DMA接收指针计算接收的字节数
			if(l_val!=0)
			{
				/*接收标志位置1*/
				//UART8_Rx_SIGN=1;	
				WirteRingbuffer_UART8Rx(DMA_UART8_Rx_Buffer,l_val);	
				//memcpy(UART2_Rx_Buffer,DMA_UART2_Rx_Buffer,UART2_Rx_Num);
				//memset(DMA_UART2_Rx_Buffer,0,sizeof(DMA_UART2_Rx_Buffer));
				HAL_UART_Receive_DMA(&UART8_Handler, DMA_UART8_Rx_Buffer, UART8_Rx_Buffer_Num); //开启DMA接收
				__HAL_UART_DISABLE_IT(&UART8_Handler, UART_IT_ERR);
				__HAL_UART_DISABLE_IT(&UART8_Handler, UART_IT_PE);
				
			}
			else
			{
				READ_REG(UART8_Handler.Instance->RDR);
				__HAL_UART_CLEAR_OREFLAG(&UART8_Handler);
			}
		}
		
		
		//发送完成中断
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
 
 
