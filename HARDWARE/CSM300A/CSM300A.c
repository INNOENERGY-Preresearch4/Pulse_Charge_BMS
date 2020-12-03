/**
  ******************************************************************************
  * @file    CSM300A.c
  * @author  YanWenlong
  * @version V1.0.0
  * @date    may-2020
  * @brief   ZLG-CSM300A驱动文件.
  *          工作模式：自定义协议转换
  *          帧头帧尾：0XA5  0X5A
  *          UART波特率：460800
  *          UART帧间隔：8
  *          CAN波特率：125K
  *          functionalities of the Universal Asynchronous Receiver Transmitter (UART) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  *
**/
#include "CSM300A.h"
#define CSM300A_VARIBLE
#include "VARIABLE.h" //变量声明
//////////////////////////////////////
UART_HandleTypeDef UART8_Handler; //UART句柄
uint8_t init_flag=0;    //初始化状态标识
uint32_t can4_re_cont=0;
uint8_t uart8_re[50]={0};
/////////////////////////////////////////////////////////////
//函数名称：Cms300A_Init
//函数作用：Cms300A上电初始化
//返回值  0成功 1失败
////////////////////////////////////////////////////////////
uint8_t Csm300A_Init(void)//初始化函数
{
	uint8_t init_data[]={0xf7,0xf8,0x01,0x3c,0x0f,0x08,0x01,0x00,0x0a,0x00,0x00,0x00,0x00,
	                       0x08,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                       0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x08,
	                       0x18,0x00,0x01,0x01,0x01,0x00,0xa5,0x5a,0x0f,0x05,0x01,0x01,0xd9};
	UART8_Init(9600);		        //串口初始化
	CAN4_CFG_0();
	CAN4_RST_0();
	delay_us(150);
	CAN4_RST_1();
	delay_ms(50);
    HAL_UART_Transmit(&UART8_Handler,init_data,sizeof(init_data),1000);
	can4_re_cont=0;
    delay_ms(200);//然后读取UART8接收数据配置是否成功				   
    if(init_flag==1)
	{
		CAN4_CFG_1();
		CAN4_RST_0();
		delay_us(150);
		CAN4_RST_1();
		delay_ms(3);
		UART8_Init(460800);
		return 0;
	}else
	{
		return 1;//失败
	}		
}
/////////////////////////////////////////////////////////////
//函数名称：Csm100TL_Init
//函数作用：Csm100TL初始化
//返回值  0成功 1失败
////////////////////////////////////////////////////////////
uint8_t Csm100TL_Init(void)//初始化函数
{
	uint8_t init_data[]={0xf7,0xf8,0x01,0x38,0x0f,0x08,0x01,0x00,0x08,0x00,0x00,0x00,0x00,
	                       0x08,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                       0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x08,0x00,0x00,0x00,
	                       0x00,0x00,0x00,0x00,0x01,0x00,0xa5,0x5a,0xc5};
	UART8_Init(9600);		        //串口初始化
	CAN4_CFG_0();
	CAN4_RST_0();
	delay_us(150);
	CAN4_RST_1();
	delay_ms(50);
    HAL_UART_Transmit(&UART8_Handler,init_data,sizeof(init_data),1000);
	can4_re_cont=0;
    delay_ms(200);//然后读取UART8接收数据配置是否成功				   
    if(init_flag==1)
	{
		CAN4_CFG_1();
		CAN4_RST_0();
		delay_us(150);
		CAN4_RST_1();
		delay_ms(3);
		UART8_Init(460800);
		return 0;
	}else
	{
		return 1;//失败
	}		
}
//初始化IO 串口8 
//bound:波特率
void UART8_Init(u32 bound)
{	
	//UART 初始化设置
	UART8_Handler.Instance=UART8;					    //USART1
	UART8_Handler.Init.BaudRate=bound;				    //波特率
	UART8_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART8_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART8_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART8_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART8_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART8_Handler);					    //HAL_UART_Init()会使能UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}
void UART8_IRQHandler(void)
{
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART8_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
        HAL_UART_Receive(&UART8_Handler,&Res,1,1000); 
		uart8_re[can4_re_cont]=Res;
		can4_re_cont++;
		if(can4_re_cont>=50)
		{
			can4_re_cont=0;
		}
        CAN4_DATA_TRANSIMIT(init_flag);			
	}
	HAL_UART_IRQHandler(&UART8_Handler);	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}
void CAN4_DATA_TRANSIMIT(u8 flag)
{
	u8 length=0,i=0,j=0;
	u32 can_id=0;
	u8 can_data[8]={0};
	if(flag==0)//初始化
	{
		if(can4_re_cont>=5)
		{
			if((uart8_re[0]==0xf7)&&(uart8_re[1]==0xf8)&&(uart8_re[2]==0x01)&&(uart8_re[3]==0x13)&&(uart8_re[4]==0x1D))
			{
				init_flag=1;//初始化成功//^异或
			}else
			{
				init_flag=0;
			}
			can4_re_cont=0;	
		}
		else
		{
			init_flag=0;
		}
	}else//正常运行
	{ 
		if(can4_re_cont==1)
		{
			if(uart8_re[0]!=START_MARK)
			{
				can4_re_cont=0;
			}
		}else if(can4_re_cont==2)
		{
			if(uart8_re[0]!=START_MARK)
			{
				can4_re_cont=0;
			}
		}else if(can4_re_cont>2)
		{
			if(can4_re_cont>=(uart8_re[1]+3))//接收完整包
			{
				if(uart8_re[uart8_re[1]+2]==STOP_MARK)//帧尾正常 开始解析CAN数据
				{
					 if(uart8_re[2]==0x08)//扩展帧
					 {
						can_id=((u32)uart8_re[3]<<24)+((u32)uart8_re[4]<<16)+((u32)uart8_re[5]<<8)+((u32)uart8_re[6]);
					    length=uart8_re[1]-5;
						for(i=0;i<length;i++)
						 {
							 can_data[i]=uart8_re[7+i];
						 }
						 for(i=length;i<8;i++)
						 {
							 can_data[i]=0;
						 }
					 }else
                     {
						can_id=((u32)uart8_re[3]<<8)+((u32)uart8_re[4]);
						length=uart8_re[1]-3; 
						 for(i=0;i<length;i++)
						 {
							 can_data[i]=uart8_re[5+i];
						 }
						 for(i=length;i<8;i++)
						 {
							 can_data[i]=0;
						 }
					 }
				     can4_re_cont=can4_re_cont-uart8_re[1]-3;	
				}else
				{
					for(i=1;i<can4_re_cont;i++)
					{
						if(uart8_re[i]==START_MARK)
						{
							break;
						}
					}
					for(j=0;j<(can4_re_cont-i);j++)
					{
						uart8_re[j]=uart8_re[i+j];
					}
					can4_re_cont=can4_re_cont-i;
				}
				
			}else
			{}
		}else
		{
		}
	}
}
/****************************************
*函数名称：CAN4_Send
*函数作用：通过CSM300A发送CAN数据  设定单次只发一包
*函数变量：id canid;data candata;length  数据长度
******************************************/
void CAN4_Send(u32 id,u8 *data,u8 length)
{
	u8 uartdata[20]={0};
	u8 i=0;
	if(length>8)
	{length=8;}
	uartdata[0]=START_MARK;//帧头
	if((id&0xffff0000)==0)//标准帧
	{
		uartdata[1]=length+3;//帧长度
		uartdata[2]=0x00;//帧类型
		uartdata[3]=(id&0xff00)>>8;//帧ID
		uartdata[4]=(id&0xff);//帧ID
		for(i=0;i<length;i++)
		{
			uartdata[5+i]=*(data+i);//数据
		}
		uartdata[5+length]=STOP_MARK;//帧尾
		HAL_UART_Transmit(&UART8_Handler,uartdata,6+length,1000);
	}else//扩展帧
	{
		uartdata[1]=length+5;//帧长度
		uartdata[2]=0x08;//帧类型
		uartdata[3]=(id&0xff000000)>>24;//帧ID
		uartdata[4]=(id&0xff0000)>>16;//帧ID
		uartdata[5]=(id&0xff00)>>8;//帧ID
		uartdata[6]=(id&0xff);//帧ID
		for(i=0;i<length;i++)
		{
			uartdata[7+i]=*(data+i);//数据
		}
		uartdata[7+length]=STOP_MARK;//帧尾
		HAL_UART_Transmit(&UART8_Handler,uartdata,8+length,1000);
	}	
}

