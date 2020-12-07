/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       Keil Development Kits
*                                              on the
*
*                                              Terminus
*
* Filename      : Signal_Collect_Process.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "Signal_Collect_Process.h"
#include "BSP_ADC.h"

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/



float Get_Pulse_Current(void)
{
//		u8 i;
		float cur;
		cur = (Cur_Measure_V_Pulse-Cur_Ref_V_Pulse)/90.0f*1000*0.985f;
		if((-100<cur)&&(cur<100))//ȥ��0��Ư�ƺͲ���
		{
			cur = 0;
		}
		return cur;

}

float Get_Filter_Current(void)
{
		volatile float cur;
		cur = (Cur_Measure_V_Filter-Cur_Ref_V_Filter)/90.0f*1000*0.985f;
		if((-100<cur)&&(cur<100))//ȥ��0��Ư�ƺͲ���
		{
			cur = 0;
		}
		return cur;
}



uint16_t CellMinValue(uint16_t data[],uint16_t n,uint8_t*num)
{
	uint16_t i = 0;
	uint16_t minvalue = 0;
	minvalue = data[0];
	*num=1;
	for(i = 1;i < n;i++)
	{		
	  if(minvalue > data[i])
	  {
			minvalue = data[i];
			*num=(i+1);		
		}							
	}
  return minvalue;	
}

//�����������ѹ���ֵ
uint16_t CellMaxValue(uint16_t data[],uint16_t n,uint8_t*num)
{
	uint16_t i = 0;
	uint16_t maxvalue = 0;
	maxvalue = data[0];
  *num=1;
	for(i = 1;i < n;i++)
	{		
	  if(maxvalue < data[i])
	  {
			maxvalue = data[i];
    	*num=(i+1);		
		}							
	}
  return maxvalue;	
}

//�����������¶���Сֵ�Լ����
int16_t TempMinValue(s16 data[],uint8_t n,uint8_t*num)
{
	u8 i = 0;
	int16_t minvalue = 1250;//0.1����
	*num=1;
	for(i = 0;i < n;i++)
	{		
    	  if(minvalue > data[i])
    	  {
    			minvalue = data[i];
    			*num=(i+1);		
    		}				
	}
  return minvalue;	
}

//�����������¶����ֵ
int16_t TempMaxValue(s16 data[],uint8_t n,uint8_t*num)
{
	u8 i = 0;
	int16_t maxvalue = -500;//0.1����
  *num=1;
	for(i = 0;i < n;i++)
	{		
  	  if(maxvalue < data[i])
  	  {
  			maxvalue = data[i];
      	*num=(i+1);		
  		}						
	}
  return maxvalue;	
}


void Vol_Collect_ByCycle(void)
{
		u16 i=0;
		memset(BMS_To_HMI.Cell_Voltage,0,sizeof(BMS_To_HMI.Cell_Voltage));
		//LTC6804��ȡ�����ѹ��NTC�ӿڵ�ѹֵ
		write_config(&g_LTC6804Handle);//����LTC6804�ؼӷ���VREG2 ֵ����
		volErr = read_cell_vol(&g_LTC6804Handle,BMS_To_HMI.Cell_Voltage);//��ȡcell��ѹֵ
	
		//�����ܵ�ѹ
		PACK_Voltage_mV=0;
		for(i=0;i<BATTERY_COUNT;i++) 
		{
			PACK_Voltage_mV += BMS_To_HMI.Cell_Voltage[i];
		}	
		BMS_To_HMI.Total_Voltage = PACK_Voltage_mV/10;   //����0.01V

		//������ֵ
		Max_CellVol=CellMaxValue(BMS_To_HMI.Cell_Voltage,BATTERY_COUNT,&Cell_Max_Num);
		Min_CellVol=CellMinValue(BMS_To_HMI.Cell_Voltage,BATTERY_COUNT,&Cell_Min_Num);
}


void Temp_Collect_ByCycle(void)
{
//		u16 i=0;
		memset(LTC6804_GpioVol,0,sizeof(LTC6804_GpioVol));
		write_config(&g_LTC6804Handle);//����LTC6804�ؼӷ���VREG2 ֵ����
		tempErr = read_aux_vol(&g_LTC6804Handle,LTC6804_GpioVol);//��ȡgpio�ڵ�ѹֵ					
		tempErr = get_temp(LTC6804_GpioVol,Pack_TempArr);//gpio�ڵ�ѹֵת�¶�

		//�¶���ֵ
		Pack_MinTemp = TempMinValue(Pack_TempArr,(uint8_t)TEMP_NUM,&Temp_Min_Num);//����0.1��
		Pack_MaxTemp =	TempMaxValue(Pack_TempArr,(uint8_t)TEMP_NUM,&Temp_Max_Num);//����0.1��

		//������¶ȼ���
		if((Pack_MaxTemp<350)&&(Pack_MinTemp>150)) //����0.1��
		{
				BMS_To_HMI.Pack_Temp = (s16)(((float)(Pack_MaxTemp+Pack_MinTemp))/2+0.5f);//����0.1��
		}
		else if(Pack_MaxTemp>=350) //����0.1��
		{
				BMS_To_HMI.Pack_Temp = Pack_MaxTemp;//����0.1��
		}
		else
		{
				BMS_To_HMI.Pack_Temp = Pack_MinTemp;//����0.1��
		}
		
}

void Pack_Cell_Balance(void)
{
//		if((Max_CellVol >= CHARG_BLANCE_ON)&&((BMS_To_HMI.Pack_Alarms[1]&ALARM_CELLVOLTAGE_LOW_PRE_BIT) == 0))  //Ҳ�����ڵ���Ƿѹʱ��ȥ���⣬�������װ���ߵ����Ƿѹ��
		if(((BMS_To_HMI.Pack_Alarms[0]&ALARM_PACKVOLTAGE_LOW_PRE_BIT) == 0)&&((BMS_To_HMI.Pack_Alarms[1]&ALARM_CELLVOLTAGE_LOW_PRE_BIT) == 0))  //���岻Ƿѹ�ҵ���鲻Ƿѹ�;���
		{
				if(need_balance==0) 
				{ 
					 if(Max_CellVol>=(Min_CellVol+BALANCE_DIFF_VOLTAGE)) 
					 {
							 need_balance=1;
							 LTC6804_balance_cell(&g_LTC6804Handle,Cell_Max_Num);
					 }
				} 
				else 
				{
					 if(Max_CellVol<(Min_CellVol+BALANCE_STOP_DIFF_VOLTAGE)) 
					 {
							 need_balance=0;
							 LTC6804_balance_cell(&g_LTC6804Handle,0);
					 }                       
				}
		}
		else
		{
				need_balance = 0;
				LTC6804_balance_cell(&g_LTC6804Handle,0);
		}
}


void Cur_Ref_Update(void)
{
		u16 i=0,j=0;		
		Filter_ADC1_Start();
		while(1)
		{
				if(ADC1_Flag==1)
				{
						i++;
						if((Cur_Measure_V_Filter >= 2164)&&(Cur_Measure_V_Filter <= 2185))
						{
								Cur_Ref_V_Filter += Cur_Measure_V_Filter;
						}
						else
						{
								Cur_Ref_V_Filter += 2176;
						}
						if(i>=100)
						{
								Cur_Ref_V_Filter = Cur_Ref_V_Filter/100;
								ADC1_Flag=0;
								break;
						}
						else
						{
								ADC1_Flag=0;
								delay_us(100);
								Filter_ADC1_Start();
						}
				}
		}
				
		Pulse_ADC3_Start();
		while(1)
		{
				j++;
				delay_us(100);
				SCB_InvalidateDCache_by_Addr((u32*)ADC3_ConvertValue,ADC3_CONVER_Time*4);
				ADC3_ConvertValueRead[0] = ADC3_ConvertValue[0];
				Cur_Measure_V_Pulse = ((float)(ADC3_ConvertValueRead[0]))/4096.0f*BMS_AVCC_Ref*Pulse_Facotr;
				if((Cur_Measure_V_Pulse >= 2164)&&(Cur_Measure_V_Pulse <= 2185))
				{
						Cur_Ref_V_Pulse += Cur_Measure_V_Pulse;
				}
				else
				{
						Cur_Ref_V_Pulse += 2176;
				}
				if(j>=400)
				{
						Cur_Ref_V_Pulse = Cur_Ref_V_Pulse/400;
						break;
				}	
		}
//		Pulse_ADC3_Start();
//		while(1)
//		{
//				if(ADC3_Flag==1)
//				{
//						j++;
//						if((Cur_Measure_V_Pulse >= 2164)&&(Cur_Measure_V_Pulse <= 2185))
//						{
//								Cur_Ref_V_Pulse += Cur_Measure_V_Pulse;
//						}
//						else
//						{
//								Cur_Ref_V_Pulse += 2175;
//						}
//						if(j>=40)
//						{
//								Cur_Ref_V_Pulse = Cur_Ref_V_Pulse/40;
//								ADC3_Flag=0;
//								break;
//						}	
//						else
//						{
//								ADC3_Flag=0;
//								Pulse_ADC3_Start();
//						}
//				}
//		}
//		Filter_ADC1_Start();
//		while(1)
//		{
//				i++;
//				delay_us(100);
//				SCB_InvalidateDCache_by_Addr((u32*)ADC1_ConvertValue,ADC1_CONVER_Time*4);
//				for(t=0;t<ADC1_CONVER_Time;t++)
//			{
//				ADC1_ConvertValueRead[t] = ADC1_ConvertValue[t];
//			}
//			ADC1_ConvertValueRead_Average=((float)(ADC1_ConvertValueRead[0]+ADC1_ConvertValueRead[1]+ADC1_ConvertValueRead[2]+ADC1_ConvertValueRead[3])/3)/4096.0f*BMS_AVCC_Ref;
//		Cur_Measure_V_Filter = ADC1_ConvertValueRead_Average*Filter_Facotr;
////				ADC3_ConvertValueRead[0] = ADC3_ConvertValue[0];
////				Cur_Measure_V_Pulse = ((float)(ADC3_ConvertValueRead[0]))/4096.0f*BMS_AVCC_Ref*Pulse_Facotr;
//				if((Cur_Measure_V_Filter >= 2164)&&(Cur_Measure_V_Filter <= 2185))
//				{
//						Cur_Ref_V_Filter += Cur_Measure_V_Filter;
//				}
//				else
//				{
//						Cur_Ref_V_Filter += 2175;
//				}
//				if(i>=10)
//				{
//						Cur_Ref_V_Filter = Cur_Ref_V_Filter/10;
//						break;
//				}	
//		}
}


void Full_Capacity_Judgment(void)//���ʱ�ж��Ƿ�������ŵ�ʱ�ж��Ƿ��������岻�Ų��ı�״̬��������ϵ����ϵ磬��SOC����995���ѳ�����1
{
//			 if((Full_Capacity_Flag==0)||((BMS_To_HMI.Battery_Status&0x01)==1))
			 if((BMS_To_HMI.Battery_Status&0x01)==1)
			 {          
						 if(Max_CellVol >= CHARG_FULL_CELL_VOLTAGE)   //
						 {
								 Full_Capacity_Num++; 
						 } 
						 else 
						 {
								 Full_Capacity_Num=0; 
						 }
						 if(Full_Capacity_Num>15) 
						 {
								 Full_Capacity_Flag = 1;
							 
								 Relay_OFF;
								 CMD_Battery_Mode = 0;
								 HAL_TIM_Base_Stop_IT(&TIM2_Handler);
								 TIM2->CNT = 0;
							 
							   Remain_SOC_mAms_Pulse = Init_SOC_mAms_Pulse;
								 Remain_SOC_mAms_Filter = Init_SOC_mAms_Filter;
							 
								 Full_Capacity_Num=0;
						 } 
				} 
//				else 
				else if((BMS_To_HMI.Battery_Status&0x04)==0x04)
				{
						 if(Max_CellVol <= CHARG_REON_CELL_VOLTAGE) //
						 {
								 Full_Capacity_Num++; 
						 } 
						 else 
						 {
								 Full_Capacity_Num=0; 
						 }
						 if(Full_Capacity_Num>10) 
						 {
								 Full_Capacity_Flag = 0;  
								 Full_Capacity_Num=0;
						 }
				}
}

void Battery_Status_Judgment(void)
{
		 //*****************************************************//
		 //���ݵ����жϵ��״
		 //*****************************************************// 
			BMS_To_HMI.Battery_Status=0;
			//���״̬
			if(SOC_Current_Filter <= (-200))   //-200mA
			{
				 BMS_To_HMI.Battery_Status|=0x01;
			}
			//�ŵ�״̬
			else if(SOC_Current_Filter >= 200) //200mA
			{
				 BMS_To_HMI.Battery_Status|=0x04;
			}		
			//����״̬
			if(Full_Capacity_Flag==1) 
			{
				 BMS_To_HMI.Battery_Status|=0x02;
			}
			//����Ԥ��
			if(BMS_To_HMI.Pack_SOC <= 200) 
			{
				 BMS_To_HMI.Battery_Status|=0x08;
			}
			//����״̬
			if(need_balance == 1) 
			{
				 BMS_To_HMI.Battery_Status|=0x10;
			}
}


void Balance_Ctrl(void)
{
			if(need_balance==1) 
			{
					LTC6804_balance_cell(&g_LTC6804Handle,Cell_Max_Num);
			}else
			{
					LTC6804_balance_cell(&g_LTC6804Handle,0);
			}
}

