/*
*********************************************************************************************************
*
*                                     				 APPLICATION
*
* Filename      : Fault_Diagnose_Protect.c
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "Fault_Diagnose_Protect.h"
#include "includes.h"

/*
*********************************************************************************************************
*                                         LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/
void Fault_Diagnose_Vol(void)
{
	//��ص�ѹ������·����  2
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_VOLTAGE_AD_BIT)==0)
	{
		 if((Min_CellVol<VOLTAGE_AD_ERR)||(volErr == (-1)))   //400mv
		 {
				 vol_error_cont++;
		 }else
		 {
				 vol_error_cont=0;
		 }
		 if(vol_error_cont>=6) 
		 {
				BMS_To_HMI.Pack_Alarms[0] |= ALARM_VOLTAGE_AD_BIT;
				vol_error_cont=0;
		 }
	}else 
	{
		 if((Min_CellVol>VOLTAGE_AD_ERR_FREE)&&(volErr == 0))   //1500mv
		 {
				 vol_error_cont++;
		 }else
		 {
				 vol_error_cont=0;
		 }
		 if(vol_error_cont>=3) 
		 {
				BMS_To_HMI.Pack_Alarms[0] &= (~(ALARM_VOLTAGE_AD_BIT));
				vol_error_cont=0;
		 }
	}
	
	//������ѹԤ��     7
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_PACKVOLTAGE_OVER_PRE_BIT)==0) 
	{          
			 if(BMS_To_HMI.Total_Voltage>=OVERPACKVOLTAGE_PRE) 
			 {
					 overpackvoltage_count++; 
			 } 
			 else 
			 {
					 overpackvoltage_count=0; 
			 }
			 if(overpackvoltage_count>=3) 
			 {
					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_PACKVOLTAGE_OVER_PRE_BIT;
					 Full_Capacity_Flag =1;
					 overpackvoltage_count=0;
			 }
	 } 
	 else 
	 {
			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERPACKVOLTAGE_BIT)==0) 
			 { 
					 if(BMS_To_HMI.Total_Voltage<=OVERPACKVOLTAGE_PRE_FREE) 
					 {
							 overpackvoltage_count++; 
					 } 
					 else 
					 {
							 overpackvoltage_count=0; 
					 }
					 if(overpackvoltage_count>=3) 
					 {
							 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_PACKVOLTAGE_OVER_PRE_BIT));  
							 overpackvoltage_count=0;
					 }
			 }
	 }
	 
	//�����ǷѹԤ��      8
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_PACKVOLTAGE_LOW_PRE_BIT)==0) 
	{          
			 if(BMS_To_HMI.Total_Voltage <= LOWPACKVOLTAGE_PRE) 
			 {
					 lowpackvoltage_count++; 
			 } 
			 else 
			 {
					 lowpackvoltage_count=0; 
			 }
			 if(lowpackvoltage_count>=3) 
			 {
					 BMS_To_HMI.Pack_Alarms[0] |= ALARM_PACKVOLTAGE_LOW_PRE_BIT;
					 lowpackvoltage_count=0;
			 }
	 } 
	 else 
	 {
       if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_LOWPACKVOLTAGE_BIT)==0) 
	     {
					 if(BMS_To_HMI.Total_Voltage >= LOWPACKVOLTAGE_PRE_FREE) 
					 {
							 lowpackvoltage_count++; 
					 } 
					 else 
					 {
							 lowpackvoltage_count=0; 
					 }
					 if(lowpackvoltage_count>=3) 
					 {
							 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_PACKVOLTAGE_LOW_PRE_BIT));  
							 lowpackvoltage_count=0;
					 }
			 }
	 }
	 
	 //��ص���ǷѹԤ��           3
	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_CELLVOLTAGE_LOW_PRE_BIT)==0) 
	 {          
			 if(Min_CellVol < CELL_LOWVOLTAGE_PRE) 
			 {
					 lowcellvoltage_count++; 
			 } 
			 else 
			 {
					 lowcellvoltage_count=0; 
			 }
			 if(lowcellvoltage_count>=3) 
			 {
					 lowcellvoltage_count=0;
					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_LOW_PRE_BIT;  
			 } 
	 } 
	 else 
	 {
			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_LOWCELLVOLTAGE_BIT)==0) 
			 { 
					 if(Min_CellVol>CELL_LOWVOLTAGE_PRE_FREE) 
					 {
							 lowcellvoltage_count++;
					 } 
					 else 
					 {
							 lowcellvoltage_count=0; 
					 }
					 if(lowcellvoltage_count>=3) 
					 {  
							 lowcellvoltage_count=0;
							 BMS_To_HMI.Pack_Alarms[1] &= (~(ALARM_CELLVOLTAGE_LOW_PRE_BIT));
					 } 
			 }
	 } 						   //
		 
	 //��ص����ѹԤ��            4
	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_CELLVOLTAGE_OVER_PRE_BIT)==0) 
	 {          
			 if(Max_CellVol>CELL_OVERVOLTAGE_PRE) 
			 {
					 overcellvoltage_count++; 
			 } 
			 else 
			 {
					 overcellvoltage_count=0; 
			 }
			 if(overcellvoltage_count>=3) 
			 {
					 overcellvoltage_count=0;
					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_OVER_PRE_BIT;
					 Full_Capacity_Flag =1;
			 } 
	 } 
	 else 
	 {
			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERCELLVOLTAGE_BIT)==0) 
			 {
					 if(Max_CellVol<CELL_OVERVOLTAGE_PRE_FREE) 
					 {
							 overcellvoltage_count++;
					 } 
					 else 
					 {
							 overcellvoltage_count=0; 
					 }
					 if(overcellvoltage_count>=3) 
					 {  
							 overcellvoltage_count=0;
							 BMS_To_HMI.Pack_Alarms[1] &= (~(ALARM_CELLVOLTAGE_OVER_PRE_BIT));           
					 }
			 }					 
	 }
	 
	 //��ص��岻ƽ��        5
	 //�����оģ�鲻ƽ����ϣ����ʱ��������������ѹ�����300mV���ŵ�ʱ��������������ѹ�����500mV������Ϊ�����оģ�鲻ƽ����ϡ�
	 if((BMS_To_HMI.Pack_Alarms[0]&ALARM_VOLTAGE_AD_BIT)==0)     //��ѹ�ɼ�����������²�ȥ�ж�
	 {
			cell_diff_blance=0;
			cell_diff_blance_free=0;
			 
			if(Max_CellVol>=(Min_CellVol+CELL_DIFF_VOLTAGE_ERR)) 
			{
					 cell_diff_blance=1;
			}
			else if(Max_CellVol<(Min_CellVol+CELL_DIFF_VOLTAGE_ERR_FREE))
			{
					 cell_diff_blance_free=1;
			}					 
			if((BMS_To_HMI.Pack_Alarms[1]&ALARM_UNBLANCE_BIT)==0) 
			{          
					 if(cell_diff_blance==1) 
					 {
							 unblance_count++; 
					 } 
					 else 
					 {
							 unblance_count=0; 
					 }
					 if(unblance_count>10) 
					 {
							 BMS_To_HMI.Pack_Alarms[1]|=ALARM_UNBLANCE_BIT;
							 unblance_count=0;
					 } 
			} 
			else 
			{
					 if(cell_diff_blance_free==1) 
					 {
							 unblance_count++; 
					 } 
					 else 
					 {
							 unblance_count=0; 
					 }
					 if(unblance_count>10) 
					 {
							 BMS_To_HMI.Pack_Alarms[1]&=(~(ALARM_UNBLANCE_BIT)); 
							 unblance_count=0;
							 
					 } 
			}
	 }
	 
	//������ѹ�жϹ���           2
	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERPACKVOLTAGE_BIT)==0) 
	 {          
			 if(BMS_To_HMI.Total_Voltage >= CUTOFF_OVERPACKVOLTAGE) 
			 {
					 cutoff_overpackvoltage_count++; 
			 } 
			 else 
			 {
					 cutoff_overpackvoltage_count=0; 
			 }
			 if(cutoff_overpackvoltage_count>=2) 
			 {
					 cutoff_overpackvoltage_count=0;
					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_OVERPACKVOLTAGE_BIT;
					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_PACKVOLTAGE_OVER_PRE_BIT;//Ԥ������Ҳ��λ
					 Full_Capacity_Flag =1;
					 RELAY_OFF();
			 } 
	 }
	 //�����Ƿѹ�жϹ���           3
	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_LOWPACKVOLTAGE_BIT)==0) 
	 {          
			 if(BMS_To_HMI.Total_Voltage <= CUTOFF_LOWPACKVOLTAGE) 
			 {
					 cutoff_lowpackvoltage_count++; 
			 } 
			 else 
			 {
					 cutoff_lowpackvoltage_count=0; 
			 }
			 if(cutoff_lowpackvoltage_count>=2) 
			 {
					 cutoff_lowpackvoltage_count=0;
					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_LOWPACKVOLTAGE_BIT;
					 BMS_To_HMI.Pack_Alarms[0] |= ALARM_PACKVOLTAGE_LOW_PRE_BIT;//Ԥ������Ҳ��λ
					 RELAY_OFF();
			 } 
	 }
	 //��ص����ѹ�жϹ���           4
	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERCELLVOLTAGE_BIT)==0) 
	 {          
			 if(Max_CellVol >= CUTOFF_OVERCELLVOLTAGE) 
			 {
					 cutoff_overcellvoltage_count++; 
			 } 
			 else 
			 {
					 cutoff_overcellvoltage_count=0; 
			 }
			 if(cutoff_overcellvoltage_count>=2) 
			 {
					 cutoff_overcellvoltage_count=0;
					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_OVERCELLVOLTAGE_BIT;
					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_OVER_PRE_BIT;//Ԥ������Ҳ��λ
					 RELAY_OFF();
			 } 
	 }
	 //��ص���Ƿѹ�жϹ���           5
	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_LOWCELLVOLTAGE_BIT)==0) 
	 {          
			 if(Min_CellVol <= CUTOFF_LOWCELLVOLTAGE) 
			 {
					 cutoff_lowcellvoltage_count++; 
			 } 
			 else 
			 {
					 cutoff_lowcellvoltage_count=0; 
			 }
			 if(cutoff_lowcellvoltage_count>=2) 
			 {
					 cutoff_lowcellvoltage_count=0;
					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_LOWCELLVOLTAGE_BIT;
					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_LOW_PRE_BIT;//Ԥ������Ҳ��λ
					 RELAY_OFF();
					 Remain_SOC_mAms_Pulse = Init_SOC_mAms_Pulse*0.1f;
					 Remain_SOC_mAms_Filter = Init_SOC_mAms_Filter*0.1f;
			 } 
	 }
}


void Fault_Diagnose_Temp(void)
{
	//����¶Ȳɼ���·����  3
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_TEMP_AD_BIT)==0)
	{
		 if(Pack_MaxTemp>(Pack_MinTemp+TEMP_AD_ERR)) //�����С��25�ȣ��ֱ���0.1��
		 {
				 temp_error_cont++;
		 } else
		 {
				 temp_error_cont=0;
		 }
		 if(temp_error_cont>=6) 
		 {
				BMS_To_HMI.Pack_Alarms[0] |= ALARM_TEMP_AD_BIT;
				temp_error_cont=0;
		 }
	}else 
	{
		 if(Pack_MaxTemp<(Pack_MinTemp+TEMP_AD_ERR_FREE)) 
		 {
				 temp_error_cont++;
		 } else
		 {
				 temp_error_cont=0;
		 }
		 if(temp_error_cont>=3) 
		 {
				BMS_To_HMI.Pack_Alarms[0] &= (~(ALARM_TEMP_AD_BIT));
				temp_error_cont=0;
		 }
	}
	
	//��������Ԥ������    5     
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_OVERTEMP_PRE_BIT)==0) 
	{          
		 if(Pack_MaxTemp>=TEMP_HIGH_PRE) 
		 {
				tempover_count++; 
		 } 
		 else 
		 {
				tempover_count=0; 
		 }
		 if(tempover_count>=3) 
		 {
				BMS_To_HMI.Pack_Alarms[0]|=ALARM_OVERTEMP_PRE_BIT;
				tempover_count=0;
		 }                      
	} 
	else 
	{
		 if(Pack_MaxTemp<=TEMP_HIGH_PRE_FREE) 
		 {
				 tempover_count++; 
		 } 
		 else 
		 {
				 tempover_count=0; 
		 }
		 if(tempover_count>=3) 
		 {
				 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_OVERTEMP_PRE_BIT));  
				 tempover_count=0;                    
		 }
	}

	//�����Ԥ������     6                
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_LOWTEMP_PRE_BIT)==0) 
	{          
			 if(Pack_MinTemp<=TEMP_LOW_PRE) 
			 {
					 templow_count++; 
			 } 
			 else 
			 {
					 templow_count=0; 
			 }
			 if(templow_count>=3) 
			 {
					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_LOWTEMP_PRE_BIT;
					 templow_count=0;
			 }
	 } 
	 else 
	 {
			 if(Pack_MinTemp>=TEMP_LOW_PRE_FREE) 
			 {
					 templow_count++; 
			 } 
			 else 
			 {
					 templow_count=0; 
			 }
			 if(templow_count>=3) 
			 {
					 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_LOWTEMP_PRE_BIT));  
					 templow_count=0;
			 }
	 }
	 
	 //�����жϹ���           1
	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERTEMP_BIT)==0) 
	 {          
			 if(Pack_MaxTemp >= CUTOFF_OVERTEMP) 
			 {
					 cutoff_tempover_count++; 
			 } 
			 else 
			 {
					 cutoff_tempover_count=0; 
			 }
			 if(cutoff_tempover_count>=2) 
			 {
					 cutoff_tempover_count=0;
					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_OVERTEMP_BIT;
					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_OVERTEMP_PRE_BIT;//Ԥ��Ҳ��λ
					 RELAY_OFF();
			 } 
	 }
	
}

void Fault_Diagnose_Discharge_COMM(void)
{
	//����λ��ͨѶ���ϼ��
	if(Host_COMM_error_flag==0) 
	{
		 if(HOST_COMM_ON==0) 
		 {
				Host_COMM_error_cont++;
		 }else
		 {
				Host_COMM_error_cont=0;
		 }
		 if(Host_COMM_error_cont>=20)     //2s
		 {
				Host_COMM_error_flag=1;
				Host_COMM_error_cont=0;
				RELAY_OFF();
		 }		 
	}
	else 
	{   
		 if(HOST_COMM_ON==1) 
		 {
				Host_COMM_error_flag=0;
				Host_COMM_error_cont=0;
		 }
	}
	HOST_COMM_ON=0;

	 //��طŵ��������Ԥ��          2
	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_DCOVERCURRENT_PRE_BIT)==0) 
	 {          
			if(SOC_Current_Filter > Discharge_Current_Over_PRE)   //5.5A
			{
					 dcurrent_count++; 
			} 
			else 
			{
					 dcurrent_count=0; 
			}
			if(dcurrent_count>=3) 
			{
					 BMS_To_HMI.Pack_Alarms[1]|=ALARM_DCOVERCURRENT_PRE_BIT; 
					 dcurrent_count=0;
			} 
	 } 
	 else 
	 {
			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_DCOVERCURRENT_BIT)==0)//���û�������ŵ��жϹ��ϣ��ſɻָ�
			 {
						if(SOC_Current_Filter < Discharge_Current_Over_PRE_Free)  //5A
						{
								 dcurrent_count++; 
						} 
						else 
						{
								 dcurrent_count=0; 
						}
						if(dcurrent_count>=3) 
						{
								 BMS_To_HMI.Pack_Alarms[1]&=(~(ALARM_DCOVERCURRENT_PRE_BIT)); 
								 dcurrent_count=0;
						} 
			 }
	 }	
	 
	 //�ŵ�����жϹ���           6
	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_DCOVERCURRENT_BIT)==0) 
	 {          
			 if(SOC_Current_Filter >= CUTOFF_Discharge_Current_Over) 
			 {
					 cutoff_dcurrent_count++; 
			 } 
			 else 
			 {
					 cutoff_dcurrent_count=0; 
			 }
			 if(cutoff_dcurrent_count>=2) 
			 {
					 cutoff_dcurrent_count=0;
					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_DCOVERCURRENT_BIT;
					 BMS_To_HMI.Pack_Alarms[1]|=ALARM_DCOVERCURRENT_PRE_BIT;
					 RELAY_OFF();
			 } 
	 }
}

void Fault_Diagnose_Charge(void)
{
	 //��س���������Ԥ��      1,���ռ�ձȲ�����С��50%��һ�����ڲ�Simple_Accu���㣬��һ��ĵ�����ֵ���Ͼ͹���
	if(SOC_Current_Pulse != 0)
	{
		 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_COVERCURRENT_PRE_BIT)==0) 
		 {          
				 if(SOC_Current_Pulse < Charge_Current_Over_PRE)   //-6A
				 {
						 ccurrent_count++; 
				 } 
				 else 
				 {
						 ccurrent_count=0; 
				 }
				 if(ccurrent_count>=3) 
				 {
						 BMS_To_HMI.Pack_Alarms[1]|=ALARM_COVERCURRENT_PRE_BIT;
						 ccurrent_count=0;
				 } 
		 } 
		 else 
		 {
				 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_COVERCURRENT_BIT)==0) 
					{ 
							 if(SOC_Current_Pulse > Charge_Current_Over_PRE_Free) //-5.5A
							 {
									 ccurrent_count++; 
							 } 
							 else 
							 {
									 ccurrent_count=0; 
							 }
							 if(ccurrent_count>=3) 
							 {
									 BMS_To_HMI.Pack_Alarms[1]&=(~(ALARM_COVERCURRENT_PRE_BIT));  
									 ccurrent_count=0;
							 }
					}
		  }
		 
		  //�������жϹ���           6
		 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_COVERCURRENT_BIT)==0) 
		 {          
				 if(SOC_Current_Filter <= CUTOFF_Charge_Current_Over) 
				 {
						 cutoff_ccurrent_count++; 
				 } 
				 else 
				 {
						 cutoff_ccurrent_count=0; 
				 }
				 if(cutoff_ccurrent_count>=2) 
				 {
						 cutoff_ccurrent_count=0;
						 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_COVERCURRENT_BIT;
						 BMS_To_HMI.Pack_Alarms[1]|=ALARM_COVERCURRENT_PRE_BIT;
						 RELAY_OFF();
				 } 
		 }
	}
}


//void Fault_Diagnose(void)
//{
//	//����λ��ͨѶ���ϼ��
//	if(Host_COMM_error_flag==0) 
//	{
//		 if(HOST_COMM_ON==0) 
//		 {
//				Host_COMM_error_cont++;
//		 }else
//		 {
//				Host_COMM_error_cont=0;
//		 }
//		 if(Host_COMM_error_cont>20)     //2s
//		 {
//				Host_COMM_error_flag=1;
//				Host_COMM_error_cont=0;
//		 }		 
//	}
//	else 
//	{   
//		 if(HOST_COMM_ON==1) 
//		 {
//				Host_COMM_error_flag=0;
//				Host_COMM_error_cont=0;
//		 }
//	}
//	HOST_COMM_ON=0; 
		
	//0
	//Ƭ��洢������  1

//	//��ص�ѹ������·����  2
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_VOLTAGE_AD_BIT)==0)
//	{
//		 if((Min_CellVol<VOLTAGE_AD_ERR)||(volErr == (-1)))   //400mv
//		 {
//				 vol_error_cont++;
//		 }else
//		 {
//				 vol_error_cont=0;
//		 }
//		 if(vol_error_cont>20) 
//		 {
//				BMS_To_HMI.Pack_Alarms[0] |= ALARM_VOLTAGE_AD_BIT;
//				vol_error_cont=0;
//		 }
//	}else 
//	{
//		 if((Min_CellVol>VOLTAGE_AD_ERR_FREE)&&(volErr == 0))   //1500mv
//		 {
//				 vol_error_cont++;
//		 }else
//		 {
//				 vol_error_cont=0;
//		 }
//		 if(vol_error_cont>10) 
//		 {
//				BMS_To_HMI.Pack_Alarms[0] &= (~(ALARM_VOLTAGE_AD_BIT));
//				vol_error_cont=0;
//		 }
//	}
		 
//	//����¶Ȳɼ���·����  3
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_TEMP_AD_BIT)==0)
//	{
//		 if(Pack_MaxTemp>(Pack_MinTemp+TEMP_AD_ERR)) //�����С��25�ȣ��ֱ���0.1��
//		 {
//				 temp_error_cont++;
//		 } else
//		 {
//				 temp_error_cont=0;
//		 }
//		 if(temp_error_cont>20) 
//		 {
//				BMS_To_HMI.Pack_Alarms[0] |= ALARM_TEMP_AD_BIT;
//				temp_error_cont=0;
//		 }
//	}else 
//	{
//		 if(Pack_MaxTemp<(Pack_MinTemp+TEMP_AD_ERR_FREE)) 
//		 {
//				 temp_error_cont++;
//		 } else
//		 {
//				 temp_error_cont=0;
//		 }
//		 if(temp_error_cont>10) 
//		 {
//				BMS_To_HMI.Pack_Alarms[0] &= (~(ALARM_TEMP_AD_BIT));
//				temp_error_cont=0;
//		 }
//	}
		 
	//��ص���������·����  4

//	//��������Ԥ������    5     
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_OVERTEMP_PRE_BIT)==0) 
//	{          
//		 if(Pack_MaxTemp>=TEMP_HIGH_PRE) 
//		 {
//				tempover_count++; 
//		 } 
//		 else 
//		 {
//				tempover_count=0; 
//		 }
//		 if(tempover_count>20) 
//		 {
//				BMS_To_HMI.Pack_Alarms[0]|=ALARM_OVERTEMP_PRE_BIT;
//				tempover_count=0;
//		 }                      
//	} 
//	else 
//	{
//		 if(Pack_MaxTemp<=TEMP_HIGH_PRE_FREE) 
//		 {
//				 tempover_count++; 
//		 } 
//		 else 
//		 {
//				 tempover_count=0; 
//		 }
//		 if(tempover_count>10) 
//		 {
//				 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_OVERTEMP_PRE_BIT));  
//				 tempover_count=0;                    
//		 }
//	}

//	//�����Ԥ������     6                
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_LOWTEMP_PRE_BIT)==0) 
//	{          
//			 if(Pack_MinTemp<=TEMP_LOW_PRE) 
//			 {
//					 templow_count++; 
//			 } 
//			 else 
//			 {
//					 templow_count=0; 
//			 }
//			 if(templow_count>20) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_LOWTEMP_PRE_BIT;
//					 templow_count=0;
//			 }
//	 } 
//	 else 
//	 {
//			 if(Pack_MinTemp>=TEMP_LOW_PRE_FREE) 
//			 {
//					 templow_count++; 
//			 } 
//			 else 
//			 {
//					 templow_count=0; 
//			 }
//			 if(templow_count>10) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_LOWTEMP_PRE_BIT));  
//					 templow_count=0;
//			 }
//	 }
		
//	//������ѹԤ��     7
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_PACKVOLTAGE_OVER_PRE_BIT)==0) 
//	{          
//			 if(BMS_To_HMI.Total_Voltage>=OVERPACKVOLTAGE_PRE) 
//			 {
//					 overpackvoltage_count++; 
//			 } 
//			 else 
//			 {
//					 overpackvoltage_count=0; 
//			 }
//			 if(overpackvoltage_count>20) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_PACKVOLTAGE_OVER_PRE_BIT;
//					 overpackvoltage_count=0;
//			 }
//	 } 
//	 else 
//	 {
//			 if(BMS_To_HMI.Total_Voltage<=OVERPACKVOLTAGE_PRE_FREE) 
//			 {
//					 overpackvoltage_count++; 
//			 } 
//			 else 
//			 {
//					 overpackvoltage_count=0; 
//			 }
//			 if(overpackvoltage_count>10) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_PACKVOLTAGE_OVER_PRE_BIT));  
//					 overpackvoltage_count=0;
//			 }
//	 }
	 
//	//�����ǷѹԤ��      8
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_PACKVOLTAGE_LOW_PRE_BIT)==0) 
//	{          
//			 if(BMS_To_HMI.Total_Voltage <= LOWPACKVOLTAGE_PRE) 
//			 {
//					 lowpackvoltage_count++; 
//			 } 
//			 else 
//			 {
//					 lowpackvoltage_count=0; 
//			 }
//			 if(lowpackvoltage_count>20) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[0] |= ALARM_PACKVOLTAGE_LOW_PRE_BIT;
//					 lowpackvoltage_count=0;
//			 }
//	 } 
//	 else 
//	 {
//			 if(BMS_To_HMI.Total_Voltage >= LOWPACKVOLTAGE_PRE_FREE) 
//			 {
//					 lowpackvoltage_count++; 
//			 } 
//			 else 
//			 {
//					 lowpackvoltage_count=0; 
//			 }
//			 if(lowpackvoltage_count>10) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[0]&=(~(ALARM_PACKVOLTAGE_LOW_PRE_BIT));  
//					 lowpackvoltage_count=0;
//			 }
//	 }
		
//	 //1
//	 //��س���������Ԥ��      1
//	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_COVERCURRENT_PRE_BIT)==0) 
//	 {          
//			 if(SOC_Current_Pulse < Charge_Current_Over_PRE)   //-3A
//			 {
//					 ccurrent_count++; 
//			 } 
//			 else 
//			 {
//					 ccurrent_count=0; 
//			 }
//			 if(ccurrent_count>10) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[1]|=ALARM_COVERCURRENT_PRE_BIT;
//					 ccurrent_count=0;
//			 } 
//	 } 
//	 else 
//	 {
//			 if(SOC_Current_Pulse > Charge_Current_Over_PRE_Free) //-2.5A
//			 {
//					 ccurrent_count++; 
//			 } 
//			 else 
//			 {
//					 ccurrent_count=0; 
//			 }
//			 if(ccurrent_count>5) 
//			 {
//					 BMS_To_HMI.Pack_Alarms[1]&=(~(ALARM_COVERCURRENT_PRE_BIT));  
//					 ccurrent_count=0;
//			 }
//	  }
			
//	 //��طŵ��������Ԥ��          2
//	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_DCOVERCURRENT_PRE_BIT)==0) 
//	 {          
//			if(SOC_Current_Filter > Discharge_Current_Over_PRE)   //10A
//			{
//					 dcurrent_count++; 
//			} 
//			else 
//			{
//					 dcurrent_count=0; 
//			}
//			if(dcurrent_count>6) 
//			{
//					 BMS_To_HMI.Pack_Alarms[1]|=ALARM_DCOVERCURRENT_PRE_BIT; 
//					 dcurrent_count=0;
//			} 
//	 } 
//	 else 
//	 {
//			if(SOC_Current_Filter < Discharge_Current_Over_PRE_Free)  //9A
//			{
//					 dcurrent_count++; 
//			} 
//			else 
//			{
//					 dcurrent_count=0; 
//			}
//			if(dcurrent_count>3) 
//			{
//					 BMS_To_HMI.Pack_Alarms[1]&=(~(ALARM_DCOVERCURRENT_PRE_BIT)); 
//					 dcurrent_count=0;
//			} 
//	 }
		 
//	 //��ص���ǷѹԤ��           3
//	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_CELLVOLTAGE_LOW_PRE_BIT)==0) 
//	 {          
//			 if(Min_CellVol < CELL_LOWVOLTAGE_PRE) 
//			 {
//					 lowcellvoltage_count++; 
//			 } 
//			 else 
//			 {
//					 lowcellvoltage_count=0; 
//			 }
//			 if(lowcellvoltage_count>10) 
//			 {
//					 lowcellvoltage_count=0;
//					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_LOW_PRE_BIT;  
//			 } 
//	 } 
//	 else 
//	 {
//			 if(Min_CellVol>CELL_LOWVOLTAGE_PRE_FREE) 
//			 {
//					 lowcellvoltage_count++;
//			 } 
//			 else 
//			 {
//					 lowcellvoltage_count=0; 
//			 }
//			 if(lowcellvoltage_count>5) 
//			 {  
//					 lowcellvoltage_count=0;
//					 BMS_To_HMI.Pack_Alarms[1] &= (~(ALARM_CELLVOLTAGE_LOW_PRE_BIT));
//			 } 
//	 } 						   //
//		 
//	 //��ص����ѹԤ��            4
//	 if((BMS_To_HMI.Pack_Alarms[1]&ALARM_CELLVOLTAGE_OVER_PRE_BIT)==0) 
//	 {          
//			 if(Max_CellVol>CELL_OVERVOLTAGE_PRE) 
//			 {
//					 overcellvoltage_count++; 
//			 } 
//			 else 
//			 {
//					 overcellvoltage_count=0; 
//			 }
//			 if(overcellvoltage_count>10) 
//			 {
//					 overcellvoltage_count=0;
//					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_OVER_PRE_BIT;
//			 } 
//	 } 
//	 else 
//	 {
//			 if(Max_CellVol<CELL_OVERVOLTAGE_PRE_FREE) 
//			 {
//					 overcellvoltage_count++;
//			 } 
//			 else 
//			 {
//					 overcellvoltage_count=0; 
//			 }
//			 if(overcellvoltage_count>5) 
//			 {  
//					 overcellvoltage_count=0;
//					 BMS_To_HMI.Pack_Alarms[1] &= (~(ALARM_CELLVOLTAGE_OVER_PRE_BIT));           
//			 } 
//	 }

//			 //��ص��岻ƽ��        5
//				//�����оģ�鲻ƽ����ϣ����ʱ��������������ѹ�����300mV���ŵ�ʱ��������������ѹ�����500mV������Ϊ�����оģ�鲻ƽ����ϡ�
//			 if((BMS_To_HMI.Pack_Alarms[0]&ALARM_VOLTAGE_AD_BIT)==0)     //��ѹ�ɼ�����������²�ȥ�ж�
//			 {
//						cell_diff_blance=0;
//						cell_diff_blance_free=0;
//						 
//						if(Max_CellVol>=(Min_CellVol+CELL_DIFF_VOLTAGE_ERR)) 
//						{
//								 cell_diff_blance=1;
//						}
//						else if(Max_CellVol<(Min_CellVol+CELL_DIFF_VOLTAGE_ERR_FREE))
//						{
//								 cell_diff_blance_free=1;
//						}					 
//						if((BMS_To_HMI.Pack_Alarms[1]&ALARM_UNBLANCE_BIT)==0) 
//						{          
//								 if(cell_diff_blance==1) 
//								 {
//										 unblance_count++; 
//								 } 
//								 else 
//								 {
//										 unblance_count=0; 
//								 }
//								 if(unblance_count>15) 
//								 {
//										 BMS_To_HMI.Pack_Alarms[1]|=ALARM_UNBLANCE_BIT;
//										 unblance_count=0;
//								 } 
//						} 
//						else 
//						{
//								 if(cell_diff_blance_free==1) 
//								 {
//										 unblance_count++; 
//								 } 
//								 else 
//								 {
//										 unblance_count=0; 
//								 }
//								 if(unblance_count>15) 
//								 {
//										 BMS_To_HMI.Pack_Alarms[1]&=(~(ALARM_UNBLANCE_BIT)); 
//										 unblance_count=0;
//										 
//								 } 
//						}
//			 }
			 
//			 //�����жϹ���           1
//			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERTEMP_BIT)==0) 
//			 {          
//						 if(Pack_MaxTemp >= CUTOFF_OVERTEMP) 
//						 {
//								 cutoff_tempover_count++; 
//						 } 
//						 else 
//						 {
//								 cutoff_tempover_count=0; 
//						 }
//						 if(cutoff_tempover_count>5) 
//						 {
//								 cutoff_tempover_count=0;
//								 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_OVERTEMP_BIT;
//						 } 
//			 } 
//			 //������ѹ�жϹ���           2
//			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERPACKVOLTAGE_BIT)==0) 
//			 {          
//						 if(BMS_To_HMI.Total_Voltage >= CUTOFF_OVERPACKVOLTAGE) 
//						 {
//								 cutoff_overpackvoltage_count++; 
//						 } 
//						 else 
//						 {
//								 cutoff_overpackvoltage_count=0; 
//						 }
//						 if(cutoff_overpackvoltage_count>5) 
//						 {
//								 cutoff_overpackvoltage_count=0;
//								 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_OVERPACKVOLTAGE_BIT;
//						 } 
//			 }
//			 //�����Ƿѹ�жϹ���           3
//			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_LOWPACKVOLTAGE_BIT)==0) 
//			 {          
//						 if(BMS_To_HMI.Total_Voltage <= CUTOFF_LOWPACKVOLTAGE) 
//						 {
//								 cutoff_lowpackvoltage_count++; 
//						 } 
//						 else 
//						 {
//								 cutoff_lowpackvoltage_count=0; 
//						 }
//						 if(cutoff_lowpackvoltage_count>5) 
//						 {
//								 cutoff_lowpackvoltage_count=0;
//								 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_LOWPACKVOLTAGE_BIT;
//						 } 
//			 }
//			 //������ѹ�жϹ���           4
//			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_OVERCELLVOLTAGE_BIT)==0) 
//			 {          
//						 if(Max_CellVol >= CUTOFF_OVERCELLVOLTAGE) 
//						 {
//								 cutoff_overcellvoltage_count++; 
//						 } 
//						 else 
//						 {
//								 cutoff_overcellvoltage_count=0; 
//						 }
//						 if(cutoff_overcellvoltage_count>5) 
//						 {
//								 cutoff_overcellvoltage_count=0;
//								 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_OVERCELLVOLTAGE_BIT;
//						 } 
//			 }
//			 //�����Ƿѹ�жϹ���           5
//			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_LOWCELLVOLTAGE_BIT)==0) 
//			 {          
//						 if(Min_CellVol <= CUTOFF_LOWCELLVOLTAGE) 
//						 {
//								 cutoff_lowcellvoltage_count++; 
//						 } 
//						 else 
//						 {
//								 cutoff_lowcellvoltage_count=0; 
//						 }
//						 if(cutoff_lowcellvoltage_count>5) 
//						 {
//								 cutoff_lowcellvoltage_count=0;
//								 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_LOWCELLVOLTAGE_BIT;
//						 } 
//			 }
//			  //�������жϹ���           6
//			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_COVERCURRENT_BIT)==0) 
//			 {          
//						 if(SOC_Current_Filter <= CUTOFF_Charge_Current_Over) 
//						 {
//								 cutoff_ccurrent_count++; 
//						 } 
//						 else 
//						 {
//								 cutoff_ccurrent_count=0; 
//						 }
//						 if(cutoff_ccurrent_count>5) 
//						 {
//								 cutoff_ccurrent_count=0;
//								 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_COVERCURRENT_BIT;
//						 } 
//			 }
//	 //�ŵ�����жϹ���           6
//	 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_DCOVERCURRENT_BIT)==0) 
//	 {          
//			 if(SOC_Current_Filter >= CUTOFF_Discharge_Current_Over) 
//			 {
//					 cutoff_dcurrent_count++; 
//			 } 
//			 else 
//			 {
//					 cutoff_dcurrent_count=0; 
//			 }
//			 if(cutoff_dcurrent_count>5) 
//			 {
//					 cutoff_dcurrent_count=0;
//					 BMS_To_HMI.Pack_Alarms[2] |= ALARM_CUTOFF_DCOVERCURRENT_BIT;
//			 } 
//	 }
//}

void Fault_Protect(void)
{
		if(((BMS_To_HMI.Battery_Status&0x01) == 1)||(CMD_Battery_Mode == CMD_Charge))//���״̬
		{
				if((BMS_To_HMI.Pack_Alarms[2]&0X2B) != 0)
				{
						RELAY_OFF();
				}
		}
		if(((BMS_To_HMI.Battery_Status&0x04) == 1)||(CMD_Battery_Mode == CMD_Discharge))//�ŵ�״̬
		{
				if((BMS_To_HMI.Pack_Alarms[2]&0x55) != 0)
				{
						RELAY_OFF();
				}
		}
}

