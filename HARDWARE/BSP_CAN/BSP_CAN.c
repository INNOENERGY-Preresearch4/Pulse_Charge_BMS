/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_CAN.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_CAN.h"
#include "includes.h"

/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

//
CAN_HandleTypeDef   CAN1_Handler;   		//CAN1���
CanTxMsgTypeDef     CAN1_TxMessage;     //CAN1������Ϣ
CanRxMsgTypeDef     CAN1_RxMessage;     //CAN1������Ϣ
//
CAN_HandleTypeDef   CAN2_Handler;   		//CAN2���
CanTxMsgTypeDef     CAN2_TxMessage;     //CAN2������Ϣ
CanRxMsgTypeDef     CAN2_RxMessage;     //CAN2������Ϣ
//
CAN_HandleTypeDef   CAN3_Handler;   		//CAN3���
CanTxMsgTypeDef     CAN3_TxMessage;     //CAN3������Ϣ
CanRxMsgTypeDef     CAN3_RxMessage;     //CAN3������Ϣ
//
Can_Msg             CAN_Rx_Message;    			//�������ݻ���
Can_Msg             CAN_Tx_Message;    			//�������ݻ���
u32 CAN_IRQ_NUM=0;

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ54M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_11tq,6,CAN_MODE_LOOPBACK);
//������Ϊ:54M/((6+11+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��;
//CAN1
u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance=CAN1; 
    CAN1_Handler.pTxMsg=&CAN1_TxMessage;     //������Ϣ
    CAN1_Handler.pRxMsg=&CAN1_RxMessage;     //������Ϣ
    CAN1_Handler.Init.Prescaler=brp;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN1_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN1_Handler.Init.SJW=tsjw;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN1_Handler.Init.ABOM=DISABLE;     //����Զ����߹���
    CAN1_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN1_Handler.Init.NART=ENABLE;      //��ֹ�����Զ����� 
    CAN1_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN1_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) return 1;   //��ʼ��
    
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32λID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32λMASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN1_FilerConf.FilterNumber=0;          //������0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN1_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) return 2;//�˲�����ʼ��
    return 0;
}

//CAN2
u8 CAN2_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN2_FilerConf;
    
    CAN2_Handler.Instance=CAN2; 
    CAN2_Handler.pTxMsg=&CAN2_TxMessage;     //������Ϣ
    CAN2_Handler.pRxMsg=&CAN2_RxMessage;     //������Ϣ
    CAN2_Handler.Init.Prescaler=brp;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN2_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN2_Handler.Init.SJW=tsjw;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN2_Handler.Init.BS1=tbs1;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN2_Handler.Init.BS2=tbs2;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN2_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN2_Handler.Init.ABOM=DISABLE;     //����Զ����߹���
    CAN2_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN2_Handler.Init.NART=ENABLE;      //��ֹ�����Զ����� 
    CAN2_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN2_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
    if(HAL_CAN_Init(&CAN2_Handler)!=HAL_OK) return 1;   //��ʼ��
    
    CAN2_FilerConf.FilterIdHigh=0X0000;     //32λID
    CAN2_FilerConf.FilterIdLow=0X0000;
    CAN2_FilerConf.FilterMaskIdHigh=0X0000; //32λMASK
    CAN2_FilerConf.FilterMaskIdLow=0X0000;  
    CAN2_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN2_FilerConf.FilterNumber=14;          //������0
    CAN2_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN2_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN2_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN2_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(&CAN2_Handler,&CAN2_FilerConf)!=HAL_OK) return 2;//�˲�����ʼ��
    return 0;
}

//CAN3
u8 CAN3_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN3_FilerConf;
    
    CAN3_Handler.Instance=CAN3; 
    CAN3_Handler.pTxMsg=&CAN3_TxMessage;     //������Ϣ
    CAN3_Handler.pRxMsg=&CAN3_RxMessage;     //������Ϣ
    CAN3_Handler.Init.Prescaler=brp;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN3_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN3_Handler.Init.SJW=tsjw;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN3_Handler.Init.BS1=tbs1;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN3_Handler.Init.BS2=tbs2;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN3_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN3_Handler.Init.ABOM=DISABLE;     //����Զ����߹���
    CAN3_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN3_Handler.Init.NART=ENABLE;      //��ֹ�����Զ����� 
    CAN3_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN3_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
    if(HAL_CAN_Init(&CAN3_Handler)!=HAL_OK) return 1;   //��ʼ��
    
    CAN3_FilerConf.FilterIdHigh=0X0000;     //32λID
    CAN3_FilerConf.FilterIdLow=0X0000;
    CAN3_FilerConf.FilterMaskIdHigh=0X0000; //32λMASK
    CAN3_FilerConf.FilterMaskIdLow=0X0000;  
    CAN3_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN3_FilerConf.FilterNumber=0;          //������20
    CAN3_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN3_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN3_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN3_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(&CAN3_Handler,&CAN3_FilerConf)!=HAL_OK) return 2;//�˲�����ʼ��
    return 0;
}



//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
	
    if(hcan->Instance==CAN1)
		{
			__HAL_RCC_CAN1_CLK_ENABLE();                //ʹ��CAN1ʱ��
			__HAL_RCC_GPIOB_CLK_ENABLE();			    //����GPIOBʱ��
			
			GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;   //
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
			GPIO_Initure.Pull=GPIO_PULLUP;              //����
			GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
			GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //����ΪCAN1
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //��ʼ��

			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//FIFO0��Ϣ�Һ��ж�����.		  		
			HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,2);    //��ռ���ȼ�1�������ȼ�2
			HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          //ʹ���ж�
		}
		else if(hcan->Instance==CAN2)
    {
			__HAL_RCC_CAN2_CLK_ENABLE();                //ʹ��CAN2ʱ��
			__HAL_RCC_GPIOB_CLK_ENABLE();			    //����GPIOBʱ��
			
			GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;   //PA11,12
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
			GPIO_Initure.Pull=GPIO_PULLUP;              //����
			GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
			GPIO_Initure.Alternate=GPIO_AF9_CAN2;       //����ΪCAN2
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //��ʼ��

			__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);//FIFO0��Ϣ�Һ��ж�����.	  
			HAL_NVIC_SetPriority(CAN2_RX0_IRQn,1,2);    //��ռ���ȼ�1�������ȼ�2
			HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);          //ʹ���ж�
    }
		else if(hcan->Instance==CAN3)
    {
			__HAL_RCC_CAN3_CLK_ENABLE();                //ʹ��CAN3ʱ��
			__HAL_RCC_GPIOB_CLK_ENABLE();			    //����GPIOBʱ��
			
			GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4;   //PA11,12
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
			GPIO_Initure.Pull=GPIO_PULLUP;              //����
			GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
			GPIO_Initure.Alternate=GPIO_AF11_CAN3;       //����ΪCAN3
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //��ʼ��

			__HAL_CAN_ENABLE_IT(&CAN3_Handler,CAN_IT_FMP0);//FIFO0��Ϣ�Һ��ж�����.	  
			HAL_NVIC_SetPriority(CAN3_RX0_IRQn,1,2);    //��ռ���ȼ�1�������ȼ�2
			HAL_NVIC_EnableIRQ(CAN3_RX0_IRQn);          //ʹ���ж�
    }
		else{}
}


//CAN1�жϷ�����
void CAN1_RX0_IRQHandler(void)
{   
    HAL_CAN_IRQHandler(&CAN1_Handler);//�˺��������CAN_Receive_IT()��������
}


//CAN2�жϷ�����
void CAN2_RX0_IRQHandler(void)
{   
    HAL_CAN_IRQHandler(&CAN2_Handler);//�˺��������CAN_Receive_IT()��������
}


//CAN3�жϷ�����
void CAN3_RX0_IRQHandler(void)
{   
    HAL_CAN_IRQHandler(&CAN3_Handler);//�˺��������CAN_Receive_IT()��������
}


//CAN�жϴ������
//�˺����ᱻCAN_Receive_IT()����
//hcan:CAN���
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
		if(hcan->Instance==CAN1)
		{
			//CAN_Receive_IT()������ر�FIFO0��Ϣ�Һ��жϣ����������Ҫ���´�
			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
			CAN_Receive_Msg(hcan,&CAN_Rx_Message);
		}
		else if(hcan->Instance==CAN2)
		{
			//CAN_Receive_IT()������ر�FIFO0��Ϣ�Һ��жϣ����������Ҫ���´�
			__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
			CAN_Receive_Msg(hcan,&CAN_Rx_Message);
		}
		else if(hcan->Instance==CAN3)
		{
			//CAN_Receive_IT()������ر�FIFO0��Ϣ�Һ��жϣ����������Ҫ���´�
			__HAL_CAN_ENABLE_IT(&CAN3_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
			CAN_Receive_Msg(hcan,&CAN_Rx_Message);
		}
		else{}
}


	
//Tx_Message����һ������
//chanle:CANͨ�� 1 2 3			     
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN_Send_Msg(u8 chanle,Can_Msg* Tx_Message)
{	
		u16 i=0;
		if(chanle==1)
		{
			//CAN1_Handler.pTxMsg->StdId=0x00000000;        //��׼��ʶ��
			CAN1_Handler.pTxMsg->ExtId=Tx_Message->Id;        //��չ��ʶ��(29λ)
			CAN1_Handler.pTxMsg->IDE=CAN_ID_EXT;//CAN_ID_EXT;    //ʹ�ñ�׼֡
			CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;//CAN_RTR_DATA;  //����֡
			CAN1_Handler.pTxMsg->DLC=Tx_Message->DLC;  //���ݳ���              
			for(i=0;i<Tx_Message->DLC;i++)
			CAN1_Handler.pTxMsg->Data[i]=Tx_Message->Data[i];
			if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //����
			return 0;
		}
		else if(chanle==2)
		{
			//CAN2_Handler.pTxMsg->StdId=0x00000000;        //��׼��ʶ��
			CAN2_Handler.pTxMsg->ExtId=Tx_Message->Id;        //��չ��ʶ��(29λ)
			CAN2_Handler.pTxMsg->IDE=CAN_ID_EXT;//CAN_ID_EXT;    //ʹ�ñ�׼֡
			CAN2_Handler.pTxMsg->RTR=CAN_RTR_DATA;//CAN_RTR_DATA;  //����֡
			CAN2_Handler.pTxMsg->DLC=Tx_Message->DLC;  //���ݳ���              
			for(i=0;i<Tx_Message->DLC;i++)
			CAN2_Handler.pTxMsg->Data[i]=Tx_Message->Data[i];
			if(HAL_CAN_Transmit(&CAN2_Handler,10)!=HAL_OK) return 1;     //����
			return 0;
		}
		else if(chanle==3)
		{	
			//CAN3_Handler.pTxMsg->StdId=0x00000000;        //��׼��ʶ��
			CAN3_Handler.pTxMsg->ExtId=Tx_Message->Id;        //��չ��ʶ��(29λ)
			CAN3_Handler.pTxMsg->IDE=CAN_ID_EXT;//CAN_ID_EXT;    //ʹ�ñ�׼֡
			CAN3_Handler.pTxMsg->RTR=CAN_RTR_DATA;//CAN_RTR_DATA;  //����֡
			CAN3_Handler.pTxMsg->DLC=Tx_Message->DLC;  //���ݳ���              
			for(i=0;i<Tx_Message->DLC;i++)
			CAN3_Handler.pTxMsg->Data[i]=Tx_Message->Data[i];
			if(HAL_CAN_Transmit(&CAN3_Handler,10)!=HAL_OK) return 1;     //����
			return 0;
		}
		else	
		{
			return 1;
		}
}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN_Receive_Msg(CAN_HandleTypeDef* hcan,Can_Msg* Rx_Message)
{		   		   
		u32 i;
		if(hcan->Instance==CAN1)
		{
			//if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,1)!=HAL_OK) return 0;//��������	
			//length
			Rx_Message->DLC=CAN1_Handler.pRxMsg->DLC;
			//data
			for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
				Rx_Message->Data[i]=CAN1_Handler.pRxMsg->Data[i];
			for(i=CAN1_Handler.pRxMsg->DLC;i<8;i++)
				Rx_Message->Data[i]=CAN1_Handler.pRxMsg->Data[i];
			//id
			if(CAN1_Handler.pRxMsg->IDE==CAN_ID_EXT)
				Rx_Message->Id=CAN1_Handler.pRxMsg->ExtId;
			else
				Rx_Message->Id=CAN1_Handler.pRxMsg->StdId;

			CAN_IRQ_NUM++;	
			return CAN1_Handler.pRxMsg->DLC;
		}
		else if(hcan->Instance==CAN2)//CAN2
		{
			//if(HAL_CAN_Receive(&CAN2_Handler,CAN_FIFO0,1)!=HAL_OK) return 0;//��������	
			//length
			Rx_Message->DLC=CAN2_Handler.pRxMsg->DLC;
			//data
			for(i=0;i<CAN2_Handler.pRxMsg->DLC;i++)
				Rx_Message->Data[i]=CAN2_Handler.pRxMsg->Data[i];
			for(i=CAN2_Handler.pRxMsg->DLC;i<8;i++)
				Rx_Message->Data[i]=CAN2_Handler.pRxMsg->Data[i];
			//id
			if(CAN2_Handler.pRxMsg->IDE==CAN_ID_EXT)
				Rx_Message->Id=CAN2_Handler.pRxMsg->ExtId;
			else
				Rx_Message->Id=CAN2_Handler.pRxMsg->StdId;

			//CAN_IRQ_NUM++;			
			return CAN2_Handler.pRxMsg->DLC;
		}
		else if(hcan->Instance==CAN3)//CAN2
		{
			//if(HAL_CAN_Receive(&CAN3_Handler,CAN_FIFO0,1)!=HAL_OK) return 0;//��������	
			//length
			Rx_Message->DLC=CAN3_Handler.pRxMsg->DLC;
			//data
			for(i=0;i<CAN3_Handler.pRxMsg->DLC;i++)
				Rx_Message->Data[i]=CAN3_Handler.pRxMsg->Data[i];
			for(i=CAN3_Handler.pRxMsg->DLC;i<8;i++)
				Rx_Message->Data[i]=CAN3_Handler.pRxMsg->Data[i];
			//id
			if(CAN3_Handler.pRxMsg->IDE==CAN_ID_EXT)
				Rx_Message->Id=CAN3_Handler.pRxMsg->ExtId;
			else
				Rx_Message->Id=CAN3_Handler.pRxMsg->StdId;	
			
			//CAN_IRQ_NUM++;
			return CAN3_Handler.pRxMsg->DLC;
		}	
		else
		{
			return 0;
		}
}
