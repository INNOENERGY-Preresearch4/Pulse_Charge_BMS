/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*                                                                    
*
* Filename      : BSP_ADC.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "BSP_ADC.h"
#include "includes.h"

/*
*********************************************************************************************************
*                                          LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

ADC_HandleTypeDef ADC1_Handler,ADC2_Handler,ADC3_Handler;//ADC句柄
DMA_HandleTypeDef ADC1TxDMA_Handler,ADC3TxDMA_Handler;//ADC句柄

uint32_t ADC1_ConvertValue[ADC1_CONVER_Time+20]={0};
uint32_t ADC1_ConvertValueRead[ADC1_CONVER_Time+20]={0};
float ADC1_ConvertValueRead_Average=0;

uint32_t ADC3_ConvertValue[ADC3_CONVER_Time+20]={0};
uint32_t ADC3_ConvertValueRead[ADC3_CONVER_Time+20]={0};
float ADC3_ConvertValueRead_Average=0;

u8 ADC1_Flag=0;
u8 ADC3_Flag=0;


/*
*********************************************************************************************************
*                                                SERVICES
*********************************************************************************************************
*/

//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void ADC1_Init(void)//滤波测量
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=ENABLE;                      //扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //开启连续转换
    ADC1_Handler.Init.NbrOfConversion=1;             //1个转换在规则序列中 也就是只转换规则序列1
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=ENABLE;             //开启DMA请求
	  ADC1_Handler.DMA_Handle=&ADC1TxDMA_Handler;
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
}

void ADC2_Init(void)//滤波基准测量
{ 
    ADC2_Handler.Instance=ADC2;
    ADC2_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    ADC2_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC2_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC2_Handler.Init.ScanConvMode=DISABLE;                      //扫描模式
    ADC2_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC2_Handler.Init.ContinuousConvMode=DISABLE;                //开启连续转换
    ADC2_Handler.Init.NbrOfConversion=1;             //3个转换在规则序列中 也就是只转换规则序列3
    ADC2_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC2_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC2_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC2_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC2_Handler.Init.DMAContinuousRequests=DISABLE;             //开启DMA请求
    HAL_ADC_Init(&ADC2_Handler);                                 //初始化 
}

void ADC3_Init(void)//脉冲测量
{ 
    ADC3_Handler.Instance=ADC3;
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=108/4=27MHZ
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC3_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC3_Handler.Init.ScanConvMode=ENABLE;                      //扫描模式
    ADC3_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC3_Handler.Init.ContinuousConvMode=ENABLE;                //开启连续转换
    ADC3_Handler.Init.NbrOfConversion=1;             //1个转换在规则序列中 也就是只转换规则序列1
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC3_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC3_Handler.Init.DMAContinuousRequests=ENABLE;             //开启DMA请求
	  ADC3_Handler.DMA_Handle=&ADC3TxDMA_Handler;
    HAL_ADC_Init(&ADC3_Handler);                                 //初始化 
}

//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		if(hadc->Instance==ADC1)//滤波测量
		{
				ADC_ChannelConfTypeDef ADC1_ChanConf;
					GPIO_InitTypeDef GPIO_Initure;
				__HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
				__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
				__HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	

				GPIO_Initure.Pin=GPIO_PIN_2;            //PA0
				GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
				GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				
				//温度1
				ADC1_ChanConf.Channel=ADC_CHANNEL_2;                        //通道
				ADC1_ChanConf.Rank=1;                                       //1个序列
				ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_56CYCLES;        //采样时间
				ADC1_ChanConf.Offset=0;                 
				HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
	
				//Tx DMA配置
				ADC1TxDMA_Handler.Instance=DMA2_Stream0;                            //数据流选择
				ADC1TxDMA_Handler.Init.Channel=DMA_CHANNEL_0;                                //通道选择
				ADC1TxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
				ADC1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
				ADC1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
				ADC1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;    //外设数据长度:16位
				ADC1TxDMA_Handler.Init.MemDataAlignment=DMA_PDATAALIGN_WORD;       //存储器数据长度:16位
				ADC1TxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //外设流控模式
				ADC1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
				ADC1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
				ADC1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
				ADC1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
				ADC1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
				ADC1TxDMA_Handler.XferCpltCallback=HAL_DMA_IRQHandler;
				
				HAL_DMA_DeInit(&ADC1TxDMA_Handler);   
				HAL_DMA_Init(&ADC1TxDMA_Handler);
			
				__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1TxDMA_Handler);    //将DMA与ADC联系起来
			
				HAL_NVIC_SetPriority(DMA2_Stream0_IRQn,1,1);
				HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
		}
		else if(hadc->Instance==ADC2)//滤波基准测量
		{
				ADC_ChannelConfTypeDef ADC2_ChanConf;
				GPIO_InitTypeDef GPIO_Initure;
				__HAL_RCC_ADC2_CLK_ENABLE();            //使能ADC2时钟
				__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
			
				GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
				GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
				GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);		
    				
				ADC2_ChanConf.Channel=ADC_CHANNEL_0; 
				ADC2_ChanConf.Rank=1;                                       //1个序列
				ADC2_ChanConf.SamplingTime=ADC_SAMPLETIME_56CYCLES;        //采样时间
				ADC2_ChanConf.Offset=0;
				HAL_ADC_ConfigChannel(&ADC2_Handler,&ADC2_ChanConf);        //通道配置
		}
		else if(hadc->Instance==ADC3)//脉冲测量
		{
				ADC_ChannelConfTypeDef ADC3_ChanConf;
				GPIO_InitTypeDef GPIO_Initure;
				__HAL_RCC_ADC3_CLK_ENABLE();            //使能ADC1时钟
				__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
				__HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能	

				GPIO_Initure.Pin=GPIO_PIN_1;            //PA0
				GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
				GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				
				//温度1
				ADC3_ChanConf.Channel=ADC_CHANNEL_1;                        //通道
				ADC3_ChanConf.Rank=1;                                       //1个序列
				ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_28CYCLES;        //采样时间
				ADC3_ChanConf.Offset=0;                 
				HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        //通道配置
	
				//Tx DMA配置
				ADC3TxDMA_Handler.Instance=DMA2_Stream1;                            //数据流选择
				ADC3TxDMA_Handler.Init.Channel=DMA_CHANNEL_2;                                //通道选择
				ADC3TxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //存储器到外设
				ADC3TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
				ADC3TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
				ADC3TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;    //外设数据长度:16位
				ADC3TxDMA_Handler.Init.MemDataAlignment=DMA_PDATAALIGN_WORD;       //存储器数据长度:16位
				ADC3TxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //外设流控模式
				ADC3TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
				ADC3TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
				ADC3TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
				ADC3TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //存储器突发单次传输
				ADC3TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //外设突发单次传输
				ADC3TxDMA_Handler.XferCpltCallback=HAL_DMA_IRQHandler;
				
				HAL_DMA_DeInit(&ADC3TxDMA_Handler);   
				HAL_DMA_Init(&ADC3TxDMA_Handler);
			
				__HAL_LINKDMA(&ADC3_Handler,DMA_Handle,ADC3TxDMA_Handler);    //将DMA与ADC联系起来
			
//				HAL_NVIC_SetPriority(DMA2_Stream1_IRQn,1,0);
//				HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
		}
		else{}
}


//获得ADC2值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_ADC2_CHANNEL0(void)   
{
    HAL_ADC_Start(&ADC2_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC2_Handler,1);                //轮询转换
   
		return (u16)HAL_ADC_GetValue(&ADC2_Handler);	            //返回最近一次ADC2规则组的转换结果
}


//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
float Get_ADC2_CHANNEL0_Average(u8 times)
{
		float ad_val=0;
		u8 t;
		for(t=0;t<times;t++)
		{
			ad_val+=Get_ADC2_CHANNEL0();
			delay_us(500);
		}
		return ((float)(ad_val/times));
}



void DMA2_Stream0_IRQHandler(void)//ADC1  滤波
{   
		u8 t;
		Relay2_OFF;
		HAL_ADC_Stop_DMA(&ADC1_Handler);
		HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
		SCB_CleanInvalidateDCache_by_Addr((u32*)ADC1_ConvertValue,ADC1_CONVER_Time*4);
		for(t=0;t<ADC1_CONVER_Time;t++)
		{
			ADC1_ConvertValueRead[t] = ADC1_ConvertValue[t];
		}
		Cur_Measure_V_Filter=((float)(ADC1_ConvertValueRead[0]+ADC1_ConvertValueRead[1]+ADC1_ConvertValueRead[2]+ADC1_ConvertValueRead[3])/4)/4096.0f*BMS_AVCC_Ref*Filter_Facotr;
		HAL_DMA_IRQHandler(&ADC1TxDMA_Handler);
		ADC1_Flag=1;
		Relay2_ON;
}

void DMA2_Stream1_IRQHandler(void)//ADC3   脉冲
{   
////	  u8 t;
//		Relay2_OFF;
////		HAL_ADC_Stop_DMA(&ADC3_Handler);
//		//HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
//		SCB_InvalidateDCache_by_Addr((u32*)ADC3_ConvertValue,ADC3_CONVER_Time*4);
////		for(t=0;t<ADC3_CONVER_Time;t++)
////		{
//			ADC3_ConvertValueRead[0] = ADC3_ConvertValue[0];
////		}
////		ADC3_ConvertValueRead_Average=((float)(ADC3_ConvertValueRead[0]))/4096.0f*BMS_AVCC_Ref;
//		Cur_Measure_V_Pulse = ((float)(ADC3_ConvertValueRead[0]))/4096.0f*BMS_AVCC_Ref*Pulse_Facotr;
//		//Cur_Measure_V_Pulse = ((float)ADC1_ConvertValueRead[0])/4096.0f*BMS_AVCC_Ref*2;
		HAL_DMA_IRQHandler(&ADC3TxDMA_Handler);
//		ADC3_Flag=1;
//		Relay2_ON;
}


//采样函数
void Filter_ADC1_Start(void)
{
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
		HAL_ADC_Start_DMA(&ADC1_Handler,ADC1_ConvertValue,ADC1_CONVER_Time*2);//第三个参数为字节数，DMA选halfword 要乘2
}

//采样函数
void Pulse_ADC3_Start(void)
{
		//HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
		HAL_ADC_Start_DMA(&ADC3_Handler,ADC3_ConvertValue,ADC3_CONVER_Time*2);//第三个参数为字节数，DMA选halfword 要乘2
}


//ADC用法 1.如果通过调用一次start转10次取平均值，adc就要设置成连续转换，dma中断中关闭adc，start函数中数据量当成u8，要*2
//2.如果adc设置成非连续，就要调用10次start取平均值，每次dma中断把数取出来，不用关adc，取10次，start函数中数据量当成u16，不乘2


