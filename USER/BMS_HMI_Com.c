/*
*********************************************************************************************************
*
*                                     			APPLICATION
*
*
* Filename      : BMS_HMI_Com.c
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  "BMS_HMI_Com.h"
#include 	"includes.h"

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
void HMI_BMS_Com_Init(void)
{
		memset(&BMS_To_HMI,0,BMS_To_HMI_Num);
		memset(&HMI_To_BMS,0,HMI_To_BMS_Num);
		BMS_To_HMI.SyncHeader1 = BMS_HMI_SyncHeader1;  //帧同步字1  0xEB
		BMS_To_HMI.SyncHeader2 = BMS_HMI_SyncHeader2;  //帧同步字2  0x90
		BMS_To_HMI.Address = BMS_To_HMI_Address;			//地址  0x01
		BMS_To_HMI.Message_ID = BMS_To_HMI_Message_ID; 	//帧识别码  0x10
}



void HMI_To_BMS_Conversion(void)
{
		static s32 	Battery_Mode_Time_Prcess=0;//指令设置时间的过程值，以s为单位
		u8 i;
		u8 *pchar;
		if(HMI_To_BMS.Message_ID == HMI_To_BMS_Message_ID_NULL)
		{
				BMS_To_HMI.Message_Num++;
				pchar = (u8 *)(&BMS_To_HMI);
				BMS_To_HMI.SumCheck = 0;
				for(i=0;i<(BMS_To_HMI_Num-1);i++) BMS_To_HMI.SumCheck += pchar[i];
				UART8_DMA_Sent((u8*)(&BMS_To_HMI),BMS_To_HMI_Num);		
		}
		else if(HMI_To_BMS.Message_ID == HMI_To_BMS_Message_ID_CMD)
		{
				if((HMI_To_BMS.Cmd_STOP&0x01)==0x01)
				{
						Relay_OFF;
						Relay_OFF;
						CMD_Battery_Mode = 0;
						Battery_Mode_Time = 0;
						HAL_TIM_Base_Stop_IT(&TIM2_Handler);
						TIM2->CNT = 0;
				}
				else
				{
						switch(HMI_To_BMS.Cmd_Mode)
						{
								//************************充电**********************	
								case HMI_To_BMS_CMD_Charge:
									Battery_Mode_Time_Prcess = ((u16)HMI_To_BMS.Cmd_ParamL+(((u16)HMI_To_BMS.Cmd_ParamH)<<8))*60;
									if((Battery_Mode_Time_Prcess >= 60)&&(Battery_Mode_Time_Prcess <= 86400))
									{
											if(BMS_To_HMI.Pack_Alarms[2]==0)
											{
													Battery_Mode_Time = Battery_Mode_Time_Prcess;
													CMD_Battery_Mode = CMD_Charge;
													Relay_ON;
													TIM2->CNT = 0;
													HAL_TIM_Base_Start_IT(&TIM2_Handler);
											}
									}
									break;
								//************************放电**********************	
								case HMI_To_BMS_CMD_Discharge:
									Battery_Mode_Time_Prcess = ((u16)HMI_To_BMS.Cmd_ParamL+(((u16)HMI_To_BMS.Cmd_ParamH)<<8))*60;
									if((Battery_Mode_Time_Prcess >= 60)&&(Battery_Mode_Time_Prcess <= 86400))
									{
											if(BMS_To_HMI.Pack_Alarms[2]==0)
											{
													Battery_Mode_Time = Battery_Mode_Time_Prcess;
													CMD_Battery_Mode = CMD_Discharge;
													Relay_ON;
													TIM2->CNT = 0;
													HAL_TIM_Base_Start_IT(&TIM2_Handler);
											}
									}
									break;

								default:
									break;
						}	
				}
			
		}
		else
		{}
}


void HMI_To_BMS_Analyze(void)
{
		u16 i;
		u16 loop;
		u8 	CheckR;
	
		if(GetRingbufferValidLen_UART8Rx() >=15)
		{
				UART8_Rx_Num = GetRingbufferValidLen_UART8Rx();
				ReadRingbuffer_UART8Rx(UART8_Rx_Buffer,UART8_Rx_Num);
				loop=0;
				while(1)
				{
						if(UART8_Rx_Buffer[loop] == BMS_HMI_SyncHeader1)  
						{			
								if(UART8_Rx_Buffer[loop+1] == BMS_HMI_SyncHeader2)
								{	
										if(UART8_Rx_Buffer[loop+2] == 0x02)
										{
												CheckR = 0;
												for(i=0;i<(HMI_To_BMS_Num-1);i++) CheckR += UART8_Rx_Buffer[loop+i];
												if(CheckR == UART8_Rx_Buffer[loop+HMI_To_BMS_Num-1])
												{		
														HOST_COMM_ON = 1;//上位机在线
														memcpy(&HMI_To_BMS, &UART8_Rx_Buffer[loop], HMI_To_BMS_Num);
														HMI_To_BMS_Conversion();
														loop+=HMI_To_BMS_Num;
												}
												else
												{
														loop+=3;
												}
										}
										else
										{
												loop+=2;
										}
								}
								else
								{
									loop++;	
								}
						}					
						else
						{
							loop++;
						}
									
						if(loop >= UART8_Rx_Num) 
						{
							break;
						}
				}
		}
}

