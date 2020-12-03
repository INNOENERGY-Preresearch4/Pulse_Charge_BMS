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
CAN_HandleTypeDef   CAN1_Handler;   		//CAN1句柄
CanTxMsgTypeDef     CAN1_TxMessage;     //CAN1发送消息
CanRxMsgTypeDef     CAN1_RxMessage;     //CAN1接收消息
//
CAN_HandleTypeDef   CAN2_Handler;   		//CAN2句柄
CanTxMsgTypeDef     CAN2_TxMessage;     //CAN2发送消息
CanRxMsgTypeDef     CAN2_RxMessage;     //CAN2接收消息
//
CAN_HandleTypeDef   CAN3_Handler;   		//CAN3句柄
CanTxMsgTypeDef     CAN3_TxMessage;     //CAN3发送消息
CanRxMsgTypeDef     CAN3_RxMessage;     //CAN3接收消息
//
Can_Msg             CAN_Rx_Message;    			//接收数据缓存
Can_Msg             CAN_Tx_Message;    			//接收数据缓存
u32 CAN_IRQ_NUM=0;

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
//Fpclk1的时钟在初始化的时候设置为54M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_11tq,6,CAN_MODE_LOOPBACK);
//则波特率为:54M/((6+11+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;
//CAN1
u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance=CAN1; 
    CAN1_Handler.pTxMsg=&CAN1_TxMessage;     //发送消息
    CAN1_Handler.pRxMsg=&CAN1_RxMessage;     //接收消息
    CAN1_Handler.Init.Prescaler=brp;    //分频系数(Fdiv)为brp+1
    CAN1_Handler.Init.Mode=mode;        //模式设置 
    CAN1_Handler.Init.SJW=tsjw;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //非时间触发通信模式 
    CAN1_Handler.Init.ABOM=DISABLE;     //软件自动离线管理
    CAN1_Handler.Init.AWUM=DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN1_Handler.Init.NART=ENABLE;      //禁止报文自动传送 
    CAN1_Handler.Init.RFLM=DISABLE;     //报文不锁定,新的覆盖旧的 
    CAN1_Handler.Init.TXFP=DISABLE;     //优先级由报文标识符决定 
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) return 1;   //初始化
    
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32位ID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32位MASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
    CAN1_FilerConf.FilterNumber=0;          //过滤器0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN1_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) return 2;//滤波器初始化
    return 0;
}

//CAN2
u8 CAN2_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN2_FilerConf;
    
    CAN2_Handler.Instance=CAN2; 
    CAN2_Handler.pTxMsg=&CAN2_TxMessage;     //发送消息
    CAN2_Handler.pRxMsg=&CAN2_RxMessage;     //接收消息
    CAN2_Handler.Init.Prescaler=brp;    //分频系数(Fdiv)为brp+1
    CAN2_Handler.Init.Mode=mode;        //模式设置 
    CAN2_Handler.Init.SJW=tsjw;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN2_Handler.Init.BS1=tbs1;         //tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN2_Handler.Init.BS2=tbs2;         //tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN2_Handler.Init.TTCM=DISABLE;     //非时间触发通信模式 
    CAN2_Handler.Init.ABOM=DISABLE;     //软件自动离线管理
    CAN2_Handler.Init.AWUM=DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN2_Handler.Init.NART=ENABLE;      //禁止报文自动传送 
    CAN2_Handler.Init.RFLM=DISABLE;     //报文不锁定,新的覆盖旧的 
    CAN2_Handler.Init.TXFP=DISABLE;     //优先级由报文标识符决定 
    if(HAL_CAN_Init(&CAN2_Handler)!=HAL_OK) return 1;   //初始化
    
    CAN2_FilerConf.FilterIdHigh=0X0000;     //32位ID
    CAN2_FilerConf.FilterIdLow=0X0000;
    CAN2_FilerConf.FilterMaskIdHigh=0X0000; //32位MASK
    CAN2_FilerConf.FilterMaskIdLow=0X0000;  
    CAN2_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
    CAN2_FilerConf.FilterNumber=14;          //过滤器0
    CAN2_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN2_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN2_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN2_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(&CAN2_Handler,&CAN2_FilerConf)!=HAL_OK) return 2;//滤波器初始化
    return 0;
}

//CAN3
u8 CAN3_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN3_FilerConf;
    
    CAN3_Handler.Instance=CAN3; 
    CAN3_Handler.pTxMsg=&CAN3_TxMessage;     //发送消息
    CAN3_Handler.pRxMsg=&CAN3_RxMessage;     //接收消息
    CAN3_Handler.Init.Prescaler=brp;    //分频系数(Fdiv)为brp+1
    CAN3_Handler.Init.Mode=mode;        //模式设置 
    CAN3_Handler.Init.SJW=tsjw;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN3_Handler.Init.BS1=tbs1;         //tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN3_Handler.Init.BS2=tbs2;         //tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN3_Handler.Init.TTCM=DISABLE;     //非时间触发通信模式 
    CAN3_Handler.Init.ABOM=DISABLE;     //软件自动离线管理
    CAN3_Handler.Init.AWUM=DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN3_Handler.Init.NART=ENABLE;      //禁止报文自动传送 
    CAN3_Handler.Init.RFLM=DISABLE;     //报文不锁定,新的覆盖旧的 
    CAN3_Handler.Init.TXFP=DISABLE;     //优先级由报文标识符决定 
    if(HAL_CAN_Init(&CAN3_Handler)!=HAL_OK) return 1;   //初始化
    
    CAN3_FilerConf.FilterIdHigh=0X0000;     //32位ID
    CAN3_FilerConf.FilterIdLow=0X0000;
    CAN3_FilerConf.FilterMaskIdHigh=0X0000; //32位MASK
    CAN3_FilerConf.FilterMaskIdLow=0X0000;  
    CAN3_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
    CAN3_FilerConf.FilterNumber=0;          //过滤器20
    CAN3_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN3_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN3_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN3_FilerConf.BankNumber=14;
    if(HAL_CAN_ConfigFilter(&CAN3_Handler,&CAN3_FilerConf)!=HAL_OK) return 2;//滤波器初始化
    return 0;
}



//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
	
    if(hcan->Instance==CAN1)
		{
			__HAL_RCC_CAN1_CLK_ENABLE();                //使能CAN1时钟
			__HAL_RCC_GPIOB_CLK_ENABLE();			    //开启GPIOB时钟
			
			GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;   //
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
			GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
			GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
			GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //复用为CAN1
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //初始化

			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//FIFO0消息挂号中断允许.		  		
			HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,2);    //抢占优先级1，子优先级2
			HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          //使能中断
		}
		else if(hcan->Instance==CAN2)
    {
			__HAL_RCC_CAN2_CLK_ENABLE();                //使能CAN2时钟
			__HAL_RCC_GPIOB_CLK_ENABLE();			    //开启GPIOB时钟
			
			GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;   //PA11,12
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
			GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
			GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
			GPIO_Initure.Alternate=GPIO_AF9_CAN2;       //复用为CAN2
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //初始化

			__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);//FIFO0消息挂号中断允许.	  
			HAL_NVIC_SetPriority(CAN2_RX0_IRQn,1,2);    //抢占优先级1，子优先级2
			HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);          //使能中断
    }
		else if(hcan->Instance==CAN3)
    {
			__HAL_RCC_CAN3_CLK_ENABLE();                //使能CAN3时钟
			__HAL_RCC_GPIOB_CLK_ENABLE();			    //开启GPIOB时钟
			
			GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4;   //PA11,12
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
			GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
			GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
			GPIO_Initure.Alternate=GPIO_AF11_CAN3;       //复用为CAN3
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //初始化

			__HAL_CAN_ENABLE_IT(&CAN3_Handler,CAN_IT_FMP0);//FIFO0消息挂号中断允许.	  
			HAL_NVIC_SetPriority(CAN3_RX0_IRQn,1,2);    //抢占优先级1，子优先级2
			HAL_NVIC_EnableIRQ(CAN3_RX0_IRQn);          //使能中断
    }
		else{}
}


//CAN1中断服务函数
void CAN1_RX0_IRQHandler(void)
{   
    HAL_CAN_IRQHandler(&CAN1_Handler);//此函数会调用CAN_Receive_IT()接收数据
}


//CAN2中断服务函数
void CAN2_RX0_IRQHandler(void)
{   
    HAL_CAN_IRQHandler(&CAN2_Handler);//此函数会调用CAN_Receive_IT()接收数据
}


//CAN3中断服务函数
void CAN3_RX0_IRQHandler(void)
{   
    HAL_CAN_IRQHandler(&CAN3_Handler);//此函数会调用CAN_Receive_IT()接收数据
}


//CAN中断处理过程
//此函数会被CAN_Receive_IT()调用
//hcan:CAN句柄
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
		if(hcan->Instance==CAN1)
		{
			//CAN_Receive_IT()函数会关闭FIFO0消息挂号中断，因此我们需要重新打开
			__HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
			CAN_Receive_Msg(hcan,&CAN_Rx_Message);
		}
		else if(hcan->Instance==CAN2)
		{
			//CAN_Receive_IT()函数会关闭FIFO0消息挂号中断，因此我们需要重新打开
			__HAL_CAN_ENABLE_IT(&CAN2_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
			CAN_Receive_Msg(hcan,&CAN_Rx_Message);
		}
		else if(hcan->Instance==CAN3)
		{
			//CAN_Receive_IT()函数会关闭FIFO0消息挂号中断，因此我们需要重新打开
			__HAL_CAN_ENABLE_IT(&CAN3_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
			CAN_Receive_Msg(hcan,&CAN_Rx_Message);
		}
		else{}
}


	
//Tx_Message发送一组数据
//chanle:CAN通道 1 2 3			     
//返回值:0,成功;
//		 其他,失败;
u8 CAN_Send_Msg(u8 chanle,Can_Msg* Tx_Message)
{	
		u16 i=0;
		if(chanle==1)
		{
			//CAN1_Handler.pTxMsg->StdId=0x00000000;        //标准标识符
			CAN1_Handler.pTxMsg->ExtId=Tx_Message->Id;        //扩展标识符(29位)
			CAN1_Handler.pTxMsg->IDE=CAN_ID_EXT;//CAN_ID_EXT;    //使用标准帧
			CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;//CAN_RTR_DATA;  //数据帧
			CAN1_Handler.pTxMsg->DLC=Tx_Message->DLC;  //数据长度              
			for(i=0;i<Tx_Message->DLC;i++)
			CAN1_Handler.pTxMsg->Data[i]=Tx_Message->Data[i];
			if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //发送
			return 0;
		}
		else if(chanle==2)
		{
			//CAN2_Handler.pTxMsg->StdId=0x00000000;        //标准标识符
			CAN2_Handler.pTxMsg->ExtId=Tx_Message->Id;        //扩展标识符(29位)
			CAN2_Handler.pTxMsg->IDE=CAN_ID_EXT;//CAN_ID_EXT;    //使用标准帧
			CAN2_Handler.pTxMsg->RTR=CAN_RTR_DATA;//CAN_RTR_DATA;  //数据帧
			CAN2_Handler.pTxMsg->DLC=Tx_Message->DLC;  //数据长度              
			for(i=0;i<Tx_Message->DLC;i++)
			CAN2_Handler.pTxMsg->Data[i]=Tx_Message->Data[i];
			if(HAL_CAN_Transmit(&CAN2_Handler,10)!=HAL_OK) return 1;     //发送
			return 0;
		}
		else if(chanle==3)
		{	
			//CAN3_Handler.pTxMsg->StdId=0x00000000;        //标准标识符
			CAN3_Handler.pTxMsg->ExtId=Tx_Message->Id;        //扩展标识符(29位)
			CAN3_Handler.pTxMsg->IDE=CAN_ID_EXT;//CAN_ID_EXT;    //使用标准帧
			CAN3_Handler.pTxMsg->RTR=CAN_RTR_DATA;//CAN_RTR_DATA;  //数据帧
			CAN3_Handler.pTxMsg->DLC=Tx_Message->DLC;  //数据长度              
			for(i=0;i<Tx_Message->DLC;i++)
			CAN3_Handler.pTxMsg->Data[i]=Tx_Message->Data[i];
			if(HAL_CAN_Transmit(&CAN3_Handler,10)!=HAL_OK) return 1;     //发送
			return 0;
		}
		else	
		{
			return 1;
		}
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN_Receive_Msg(CAN_HandleTypeDef* hcan,Can_Msg* Rx_Message)
{		   		   
		u32 i;
		if(hcan->Instance==CAN1)
		{
			//if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,1)!=HAL_OK) return 0;//接收数据	
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
			//if(HAL_CAN_Receive(&CAN2_Handler,CAN_FIFO0,1)!=HAL_OK) return 0;//接收数据	
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
			//if(HAL_CAN_Receive(&CAN3_Handler,CAN_FIFO0,1)!=HAL_OK) return 0;//接收数据	
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
