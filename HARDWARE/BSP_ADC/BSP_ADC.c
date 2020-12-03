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

ADC_HandleTypeDef ADC1_Handler,ADC2_Handler,ADC3_Handler;//ADC���
DMA_HandleTypeDef ADC1TxDMA_Handler,ADC3TxDMA_Handler;//ADC���

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

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void ADC1_Init(void)//�˲�����
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=ENABLE;                      //ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //��������ת��
    ADC1_Handler.Init.NbrOfConversion=1;             //1��ת���ڹ��������� Ҳ����ֻת����������1
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=ENABLE;             //����DMA����
	  ADC1_Handler.DMA_Handle=&ADC1TxDMA_Handler;
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
}

void ADC2_Init(void)//�˲���׼����
{ 
    ADC2_Handler.Instance=ADC2;
    ADC2_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADC2_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC2_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC2_Handler.Init.ScanConvMode=DISABLE;                      //ɨ��ģʽ
    ADC2_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC2_Handler.Init.ContinuousConvMode=DISABLE;                //��������ת��
    ADC2_Handler.Init.NbrOfConversion=1;             //3��ת���ڹ��������� Ҳ����ֻת����������3
    ADC2_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC2_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC2_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC2_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC2_Handler.Init.DMAContinuousRequests=DISABLE;             //����DMA����
    HAL_ADC_Init(&ADC2_Handler);                                 //��ʼ�� 
}

void ADC3_Init(void)//�������
{ 
    ADC3_Handler.Instance=ADC3;
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC3_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC3_Handler.Init.ScanConvMode=ENABLE;                      //ɨ��ģʽ
    ADC3_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC3_Handler.Init.ContinuousConvMode=ENABLE;                //��������ת��
    ADC3_Handler.Init.NbrOfConversion=1;             //1��ת���ڹ��������� Ҳ����ֻת����������1
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC3_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC3_Handler.Init.DMAContinuousRequests=ENABLE;             //����DMA����
	  ADC3_Handler.DMA_Handle=&ADC3TxDMA_Handler;
    HAL_ADC_Init(&ADC3_Handler);                                 //��ʼ�� 
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		if(hadc->Instance==ADC1)//�˲�����
		{
				ADC_ChannelConfTypeDef ADC1_ChanConf;
					GPIO_InitTypeDef GPIO_Initure;
				__HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
				__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
				__HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	

				GPIO_Initure.Pin=GPIO_PIN_2;            //PA0
				GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
				GPIO_Initure.Pull=GPIO_NOPULL;          //����������
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				
				//�¶�1
				ADC1_ChanConf.Channel=ADC_CHANNEL_2;                        //ͨ��
				ADC1_ChanConf.Rank=1;                                       //1������
				ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_56CYCLES;        //����ʱ��
				ADC1_ChanConf.Offset=0;                 
				HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
				//Tx DMA����
				ADC1TxDMA_Handler.Instance=DMA2_Stream0;                            //������ѡ��
				ADC1TxDMA_Handler.Init.Channel=DMA_CHANNEL_0;                                //ͨ��ѡ��
				ADC1TxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
				ADC1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
				ADC1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
				ADC1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;    //�������ݳ���:16λ
				ADC1TxDMA_Handler.Init.MemDataAlignment=DMA_PDATAALIGN_WORD;       //�洢�����ݳ���:16λ
				ADC1TxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //��������ģʽ
				ADC1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
				ADC1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
				ADC1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
				ADC1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
				ADC1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
				ADC1TxDMA_Handler.XferCpltCallback=HAL_DMA_IRQHandler;
				
				HAL_DMA_DeInit(&ADC1TxDMA_Handler);   
				HAL_DMA_Init(&ADC1TxDMA_Handler);
			
				__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1TxDMA_Handler);    //��DMA��ADC��ϵ����
			
				HAL_NVIC_SetPriority(DMA2_Stream0_IRQn,1,1);
				HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
		}
		else if(hadc->Instance==ADC2)//�˲���׼����
		{
				ADC_ChannelConfTypeDef ADC2_ChanConf;
				GPIO_InitTypeDef GPIO_Initure;
				__HAL_RCC_ADC2_CLK_ENABLE();            //ʹ��ADC2ʱ��
				__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
			
				GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
				GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
				GPIO_Initure.Pull=GPIO_NOPULL;          //����������
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);		
    				
				ADC2_ChanConf.Channel=ADC_CHANNEL_0; 
				ADC2_ChanConf.Rank=1;                                       //1������
				ADC2_ChanConf.SamplingTime=ADC_SAMPLETIME_56CYCLES;        //����ʱ��
				ADC2_ChanConf.Offset=0;
				HAL_ADC_ConfigChannel(&ADC2_Handler,&ADC2_ChanConf);        //ͨ������
		}
		else if(hadc->Instance==ADC3)//�������
		{
				ADC_ChannelConfTypeDef ADC3_ChanConf;
				GPIO_InitTypeDef GPIO_Initure;
				__HAL_RCC_ADC3_CLK_ENABLE();            //ʹ��ADC1ʱ��
				__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
				__HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	

				GPIO_Initure.Pin=GPIO_PIN_1;            //PA0
				GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
				GPIO_Initure.Pull=GPIO_NOPULL;          //����������
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				
				//�¶�1
				ADC3_ChanConf.Channel=ADC_CHANNEL_1;                        //ͨ��
				ADC3_ChanConf.Rank=1;                                       //1������
				ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_28CYCLES;        //����ʱ��
				ADC3_ChanConf.Offset=0;                 
				HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        //ͨ������
	
				//Tx DMA����
				ADC3TxDMA_Handler.Instance=DMA2_Stream1;                            //������ѡ��
				ADC3TxDMA_Handler.Init.Channel=DMA_CHANNEL_2;                                //ͨ��ѡ��
				ADC3TxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;             //�洢��������
				ADC3TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
				ADC3TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
				ADC3TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;    //�������ݳ���:16λ
				ADC3TxDMA_Handler.Init.MemDataAlignment=DMA_PDATAALIGN_WORD;       //�洢�����ݳ���:16λ
				ADC3TxDMA_Handler.Init.Mode=DMA_CIRCULAR;                            //��������ģʽ
				ADC3TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
				ADC3TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
				ADC3TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
				ADC3TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
				ADC3TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
				ADC3TxDMA_Handler.XferCpltCallback=HAL_DMA_IRQHandler;
				
				HAL_DMA_DeInit(&ADC3TxDMA_Handler);   
				HAL_DMA_Init(&ADC3TxDMA_Handler);
			
				__HAL_LINKDMA(&ADC3_Handler,DMA_Handle,ADC3TxDMA_Handler);    //��DMA��ADC��ϵ����
			
//				HAL_NVIC_SetPriority(DMA2_Stream1_IRQn,1,0);
//				HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
		}
		else{}
}


//���ADC2ֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_ADC2_CHANNEL0(void)   
{
    HAL_ADC_Start(&ADC2_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC2_Handler,1);                //��ѯת��
   
		return (u16)HAL_ADC_GetValue(&ADC2_Handler);	            //�������һ��ADC2�������ת�����
}


//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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



void DMA2_Stream0_IRQHandler(void)//ADC1  �˲�
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

void DMA2_Stream1_IRQHandler(void)//ADC3   ����
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


//��������
void Filter_ADC1_Start(void)
{
		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
		HAL_ADC_Start_DMA(&ADC1_Handler,ADC1_ConvertValue,ADC1_CONVER_Time*2);//����������Ϊ�ֽ�����DMAѡhalfword Ҫ��2
}

//��������
void Pulse_ADC3_Start(void)
{
		//HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
		HAL_ADC_Start_DMA(&ADC3_Handler,ADC3_ConvertValue,ADC3_CONVER_Time*2);//����������Ϊ�ֽ�����DMAѡhalfword Ҫ��2
}


//ADC�÷� 1.���ͨ������һ��startת10��ȡƽ��ֵ��adc��Ҫ���ó�����ת����dma�ж��йر�adc��start����������������u8��Ҫ*2
//2.���adc���óɷ���������Ҫ����10��startȡƽ��ֵ��ÿ��dma�жϰ���ȡ���������ù�adc��ȡ10�Σ�start����������������u16������2


