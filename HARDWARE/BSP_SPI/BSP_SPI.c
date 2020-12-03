/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_SPI.c
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_SPI.h"

/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

//SPI_HandleTypeDef SPI1_Handler;  //SPI1��� AT25M01�洢
SPI_HandleTypeDef SPI2_Handler;  //SPI2��� LTC6804�����ѹ�ɼ�
//SPI_HandleTypeDef SPI5_Handler;  //SPI5��� ������ѹ�ɼ�

/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/



//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��  3.3MHZ
//void SPI1_Init(void)
//{
//    SPI1_Handler.Instance=SPI1;                      //SP2
//    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;          //����SPI����ģʽ������Ϊ��ģʽ
//    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
//    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;    //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
//    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
//    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
//    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //�ر�TIģʽ
//    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
//    SPI1_Handler.Init.CRCPolynomial=7;               //CRCֵ����Ķ���ʽ
//    HAL_SPI_Init(&SPI1_Handler);
//    
//    __HAL_SPI_ENABLE(&SPI1_Handler);                 //ʹ��SPI1
//    SPI_ReadWriteByte(1,0Xff);                        //��������
//}



//SPI�ڳ�ʼ��
//�������Ƕ�SPI4�ĳ�ʼ�� 3.3MHZ
//void SPI5_Init(void)
//{
//    SPI5_Handler.Instance=SPI4;                      //SP2
//    SPI5_Handler.Init.Mode=SPI_MODE_MASTER;          //����SPI����ģʽ������Ϊ��ģʽ
//    SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
//    SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;    //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//    SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//    SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//    SPI5_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
//    SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
//    SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
//    SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //�ر�TIģʽ
//    SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
//    SPI5_Handler.Init.CRCPolynomial=7;               //CRCֵ����Ķ���ʽ
//    HAL_SPI_Init(&SPI5_Handler);
//    
//    __HAL_SPI_ENABLE(&SPI5_Handler);                 //ʹ��SPI1
//    SPI_ReadWriteByte(4,0Xff);                        //��������
//}



//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��  800k
void SPI2_Init(void)
{
    SPI2_Handler.Instance=SPI2;                      //SP2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;          //����SPI����ģʽ������Ϊ��ģʽ
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_16BIT;    //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI2_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //�ر�TIģʽ
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI2_Handler.Init.CRCPolynomial=7;               //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI2_Handler);
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                 //ʹ��SPI2
    //SPI_ReadWriteByte(5,0Xff);                        //��������
}



//SPI2�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	
    GPIO_InitTypeDef GPIO_Initure;
	
    if(hspi->Instance==SPI1)
	{
//		__HAL_RCC_GPIOD_CLK_ENABLE();                   //ʹ��GPIOFʱ��
//		__HAL_RCC_GPIOG_CLK_ENABLE();                   //ʹ��GPIOFʱ��
//		__HAL_RCC_SPI1_CLK_ENABLE();                    //ʹ��SPI2ʱ��
//		
//		//PB13,14,15
//		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_11;
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
//		GPIO_Initure.Pull=GPIO_PULLUP;                  //����
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����    
//		GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //����ΪSPI2
//		HAL_GPIO_Init(GPIOG,&GPIO_Initure);             //��ʼ��
//		//PB13,14,15
//		GPIO_Initure.Pin=GPIO_PIN_7;
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
//		GPIO_Initure.Pull=GPIO_PULLUP;                  //����
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����    
//		GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //����ΪSPI2
//		HAL_GPIO_Init(GPIOD,&GPIO_Initure);             //��ʼ��
	}
	else if(hspi->Instance==SPI2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();                   //ʹ��GPIOFʱ��
		__HAL_RCC_SPI2_CLK_ENABLE();                    //ʹ��SPI2ʱ��
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;                  //����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����    
		GPIO_Initure.Alternate=GPIO_AF5_SPI2;           //����ΪSPI2
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);             //��ʼ��
	}
	else if(hspi->Instance==SPI5)
	{
//		__HAL_RCC_GPIOF_CLK_ENABLE();                   //ʹ��GPIOFʱ��
//		__HAL_RCC_SPI5_CLK_ENABLE();                    //ʹ��SPI2ʱ��
//		
//		//PB13,14,15
//		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //�����������
//		GPIO_Initure.Pull=GPIO_PULLUP;                  //����
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //����    
//		GPIO_Initure.Alternate=GPIO_AF5_SPI5;           //����ΪSPI2
//		HAL_GPIO_Init(GPIOF,&GPIO_Initure);             //��ʼ��
	}
	else{}
}

//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ54Mhz��
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&SPI2_Handler);            //�ر�SPI
    SPI2_Handler.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&SPI2_Handler);             //ʹ��SPI
    
}

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//chanle :1  SPI1��� AT25M01�洢  2  SPI2��� LTC6804�����ѹ�ɼ�  5   SPI5��� ������ѹ�ɼ�
//����ֵ:��ȡ�����ֽ�
u16 SPI2_ReadWriteWord(u16 TxData)
{
		u16 Rxdata=0xFFFF;
		HAL_SPI_TransmitReceive(&SPI2_Handler,(uint8_t *)&TxData,(uint8_t *)&Rxdata,1, 10); 
		return Rxdata;          		    //�����յ�������		
}

