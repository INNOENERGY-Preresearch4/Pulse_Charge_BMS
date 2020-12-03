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

//SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄 AT25M01存储
SPI_HandleTypeDef SPI2_Handler;  //SPI2句柄 LTC6804单体电压采集
//SPI_HandleTypeDef SPI5_Handler;  //SPI5句柄 电流电压采集

/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/



//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化  3.3MHZ
//void SPI1_Init(void)
//{
//    SPI1_Handler.Instance=SPI1;                      //SP2
//    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;          //设置SPI工作模式，设置为主模式
//    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;//设置SPI单向或者双向的数据模式:SPI设置为双线模式
//    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;    //设置SPI的数据大小:SPI发送接收8位帧结构
//    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //串行同步时钟的空闲状态为高电平
//    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//定义波特率预分频的值:波特率预分频值为256
//    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
//    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //关闭TI模式
//    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
//    SPI1_Handler.Init.CRCPolynomial=7;               //CRC值计算的多项式
//    HAL_SPI_Init(&SPI1_Handler);
//    
//    __HAL_SPI_ENABLE(&SPI1_Handler);                 //使能SPI1
//    SPI_ReadWriteByte(1,0Xff);                        //启动传输
//}



//SPI口初始化
//这里针是对SPI4的初始化 3.3MHZ
//void SPI5_Init(void)
//{
//    SPI5_Handler.Instance=SPI4;                      //SP2
//    SPI5_Handler.Init.Mode=SPI_MODE_MASTER;          //设置SPI工作模式，设置为主模式
//    SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;//设置SPI单向或者双向的数据模式:SPI设置为双线模式
//    SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;    //设置SPI的数据大小:SPI发送接收8位帧结构
//    SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //串行同步时钟的空闲状态为高电平
//    SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//    SPI5_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//    SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//定义波特率预分频的值:波特率预分频值为256
//    SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
//    SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //关闭TI模式
//    SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
//    SPI5_Handler.Init.CRCPolynomial=7;               //CRC值计算的多项式
//    HAL_SPI_Init(&SPI5_Handler);
//    
//    __HAL_SPI_ENABLE(&SPI5_Handler);                 //使能SPI1
//    SPI_ReadWriteByte(4,0Xff);                        //启动传输
//}



//SPI口初始化
//这里针是对SPI2的初始化  800k
void SPI2_Init(void)
{
    SPI2_Handler.Instance=SPI2;                      //SP2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;          //设置SPI工作模式，设置为主模式
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;//设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_16BIT;    //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH; //串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI2_Handler.Init.NSS=SPI_NSS_SOFT;              //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;     //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;     //关闭TI模式
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI2_Handler.Init.CRCPolynomial=7;               //CRC值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                 //使能SPI2
    //SPI_ReadWriteByte(5,0Xff);                        //启动传输
}



//SPI2底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	
    GPIO_InitTypeDef GPIO_Initure;
	
    if(hspi->Instance==SPI1)
	{
//		__HAL_RCC_GPIOD_CLK_ENABLE();                   //使能GPIOF时钟
//		__HAL_RCC_GPIOG_CLK_ENABLE();                   //使能GPIOF时钟
//		__HAL_RCC_SPI1_CLK_ENABLE();                    //使能SPI2时钟
//		
//		//PB13,14,15
//		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_11;
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
//		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速    
//		GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //复用为SPI2
//		HAL_GPIO_Init(GPIOG,&GPIO_Initure);             //初始化
//		//PB13,14,15
//		GPIO_Initure.Pin=GPIO_PIN_7;
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
//		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速    
//		GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //复用为SPI2
//		HAL_GPIO_Init(GPIOD,&GPIO_Initure);             //初始化
	}
	else if(hspi->Instance==SPI2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();                   //使能GPIOF时钟
		__HAL_RCC_SPI2_CLK_ENABLE();                    //使能SPI2时钟
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速    
		GPIO_Initure.Alternate=GPIO_AF5_SPI2;           //复用为SPI2
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);             //初始化
	}
	else if(hspi->Instance==SPI5)
	{
//		__HAL_RCC_GPIOF_CLK_ENABLE();                   //使能GPIOF时钟
//		__HAL_RCC_SPI5_CLK_ENABLE();                    //使能SPI2时钟
//		
//		//PB13,14,15
//		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
//		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
//		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
//		GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速    
//		GPIO_Initure.Alternate=GPIO_AF5_SPI5;           //复用为SPI2
//		HAL_GPIO_Init(GPIOF,&GPIO_Initure);             //初始化
	}
	else{}
}

//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为54Mhz：
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI2_Handler);            //关闭SPI
    SPI2_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI2_Handler);             //使能SPI
    
}

//SPI2 读写一个字节
//TxData:要写入的字节
//chanle :1  SPI1句柄 AT25M01存储  2  SPI2句柄 LTC6804单体电压采集  5   SPI5句柄 电流电压采集
//返回值:读取到的字节
u16 SPI2_ReadWriteWord(u16 TxData)
{
		u16 Rxdata=0xFFFF;
		HAL_SPI_TransmitReceive(&SPI2_Handler,(uint8_t *)&TxData,(uint8_t *)&Rxdata,1, 10); 
		return Rxdata;          		    //返回收到的数据		
}

