#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/6/23
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	

UART_HandleTypeDef UART5_Handler; //UART���


//��ʼ��IO ����5 
//bound:������
void UART5_Init(u32 bound)
{	
	//UART ��ʼ������
	UART5_Handler.Instance=UART5;					    //USART1
	UART5_Handler.Init.BaudRate=bound;				    //������
	UART5_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART5_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART5_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART5_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART5_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART5_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}else if(huart->Instance==UART5)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_UART5_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_2;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART5;	//����ΪUSART1
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_12;			//PA10
		GPIO_Initure.Alternate=GPIO_AF8_UART5;	//����ΪUSART1
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//��ʼ��PA10
		
#if EN_UART5_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(UART5_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(UART5_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}else if(huart->Instance==UART8)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_UART8_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART8;	//����ΪUSART1
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//��ʼ��PA9		
#if EN_UART8_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(UART8_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(UART8_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	

}
void UART5_IRQHandler(void)
{
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART5_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        HAL_UART_Receive(&UART5_Handler,&Res,1,1000); 
		
	}
	HAL_UART_IRQHandler(&UART5_Handler);	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}
	


/****************************************************************************************/
/****************************************************************************************/
/*************************�������ͨ���ڻص������б�д�жϿ����߼�*********************/
/****************************************************************************************
***************************************************************************************************
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
}
 
//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif	
*/



 




