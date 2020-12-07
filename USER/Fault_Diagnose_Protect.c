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
	//电池电压采样电路故障  2
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
	
	//电池组过压预警     7
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
	 
	//电池组欠压预警      8
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
	 
	 //电池单体欠压预警           3
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
		 
	 //电池单体过压预警            4
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
	 
	 //电池单体不平衡        5
	 //单体电芯模块不平衡故障：充电时任意两个单体间电压差大于300mV，放电时任意两个单体间电压差大于500mV，则认为单体电芯模块不平衡故障。
	 if((BMS_To_HMI.Pack_Alarms[0]&ALARM_VOLTAGE_AD_BIT)==0)     //电压采集正常的情况下才去判断
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
	 
	//电池组过压切断故障           2
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
					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_PACKVOLTAGE_OVER_PRE_BIT;//预警故障也置位
					 Full_Capacity_Flag =1;
					 RELAY_OFF();
			 } 
	 }
	 //电池组欠压切断故障           3
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
					 BMS_To_HMI.Pack_Alarms[0] |= ALARM_PACKVOLTAGE_LOW_PRE_BIT;//预警故障也置位
					 RELAY_OFF();
			 } 
	 }
	 //电池单体过压切断故障           4
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
					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_OVER_PRE_BIT;//预警故障也置位
					 RELAY_OFF();
			 } 
	 }
	 //电池单体欠压切断故障           5
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
					 BMS_To_HMI.Pack_Alarms[1] |= ALARM_CELLVOLTAGE_LOW_PRE_BIT;//预警故障也置位
					 RELAY_OFF();
					 Remain_SOC_mAms_Pulse = Init_SOC_mAms_Pulse*0.1f;
					 Remain_SOC_mAms_Filter = Init_SOC_mAms_Filter*0.1f;
			 } 
	 }
}


void Fault_Diagnose_Temp(void)
{
	//电池温度采集电路故障  3
	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_TEMP_AD_BIT)==0)
	{
		 if(Pack_MaxTemp>(Pack_MinTemp+TEMP_AD_ERR)) //最大最小差25度，分辨率0.1℃
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
	
	//电池组过热预警故障    5     
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

	//电池组预警过冷     6                
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
	 
	 //过热切断故障           1
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
					 BMS_To_HMI.Pack_Alarms[0]|=ALARM_OVERTEMP_PRE_BIT;//预警也置位
					 RELAY_OFF();
			 } 
	 }
	
}

void Fault_Diagnose_Discharge_COMM(void)
{
	//与上位机通讯故障检测
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

	 //电池放电电流过大预警          2
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
			 if((BMS_To_HMI.Pack_Alarms[2]&ALARM_CUTOFF_DCOVERCURRENT_BIT)==0)//如果没发生过放电切断故障，才可恢复
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
	 
	 //放电过流切断故障           6
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
	 //电池充电电流过大预警      1,充电占空比不可能小于50%，一个周期采Simple_Accu个点，有一半的点在阈值以上就过流
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
		 
		  //充电过流切断故障           6
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
//	//与上位机通讯故障检测
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
	//片外存储器故障  1

//	//电池电压采样电路故障  2
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
		 
//	//电池温度采集电路故障  3
//	if((BMS_To_HMI.Pack_Alarms[0]&ALARM_TEMP_AD_BIT)==0)
//	{
//		 if(Pack_MaxTemp>(Pack_MinTemp+TEMP_AD_ERR)) //最大最小差25度，分辨率0.1℃
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
		 
	//电池电流采样电路故障  4

//	//电池组过热预警故障    5     
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

//	//电池组预警过冷     6                
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
		
//	//电池组过压预警     7
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
	 
//	//电池组欠压预警      8
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
//	 //电池充电电流过大预警      1
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
			
//	 //电池放电电流过大预警          2
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
		 
//	 //电池单体欠压预警           3
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
//	 //电池单体过压预警            4
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

//			 //电池单体不平衡        5
//				//单体电芯模块不平衡故障：充电时任意两个单体间电压差大于300mV，放电时任意两个单体间电压差大于500mV，则认为单体电芯模块不平衡故障。
//			 if((BMS_To_HMI.Pack_Alarms[0]&ALARM_VOLTAGE_AD_BIT)==0)     //电压采集正常的情况下才去判断
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
			 
//			 //过热切断故障           1
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
//			 //电池组过压切断故障           2
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
//			 //电池组欠压切断故障           3
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
//			 //电池组过压切断故障           4
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
//			 //电池组欠压切断故障           5
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
//			  //充电过流切断故障           6
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
//	 //放电过流切断故障           6
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
		if(((BMS_To_HMI.Battery_Status&0x01) == 1)||(CMD_Battery_Mode == CMD_Charge))//充电状态
		{
				if((BMS_To_HMI.Pack_Alarms[2]&0X2B) != 0)
				{
						RELAY_OFF();
				}
		}
		if(((BMS_To_HMI.Battery_Status&0x04) == 1)||(CMD_Battery_Mode == CMD_Discharge))//放电状态
		{
				if((BMS_To_HMI.Pack_Alarms[2]&0x55) != 0)
				{
						RELAY_OFF();
				}
		}
}

