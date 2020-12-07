/*
*********************************************************************************************************
*
*                                     				 APPLICATION
*
* Filename      : SOC_Calculate.c
* Version       : V1.00
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


#include "SOC_Calculate.h"



/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

volatile float Filter_SOC_Change=0;
volatile float Pulse_SOC_Change=0;
volatile float Remain_SOC_mAms_Pulse_Mod=0;
volatile float Remain_SOC_mAms_Filter_Mod=0;

/*
*********************************************************************************************************
*                                              SERVICES
*********************************************************************************************************
*/




//*********************************************************
//˵��:SOC�����ʼ��
//������battery_capacity,��ʱ����ֵ��Χ0-1193����λ��ʱ
//********************************************************* 
void SOC_Calculate_Init(void)
{   
		//�˲�
    u8 SOC_Remain_Mark_Filter=0;//��ȡ����SOCд���־
    u32 Address_Midle_Filter=0;//SOCд���־����ַ
    uint8_t Count_Mark_Filter=1;//��ѯ����
	
		//���˲�
    u8 SOC_Remain_Mark_Pulse=0;//��ȡ����SOCд���־
    u32 Address_Midle_Pulse=0;//SOCд���־����ַ
    uint8_t Count_Mark_Pulse=1;//��ѯ����
	
		//�˲�
    //��EEPROM�ж�ȡ��������
    //��ȡʣ������
    Address_Midle_Filter = (Address_SOC_Remain_Filter+8);
    SOC_Remain_Mark_Filter = EEPROM_Read_Byte(&g_EEPROMSPIHandle,Address_Midle_Filter);
    
    while(SOC_Remain_Mark_Filter != 0x55) 
    {
				 if(Count_Mark_Filter>=(SOC_Storage_MAX_Num*2)) 
				 {
						 Count_Mark_Filter=0;
						 break;
				 }       
				 if(Address_Midle_Filter>=(Address_SOC_Remain_Filter+(SOC_Storage_MAX_Num*9)-1)) 
				 {
							Address_Midle_Filter= (Address_SOC_Remain_Filter+8);
				 } 
				 else 
				 {
							Address_Midle_Filter= (Address_Midle_Filter+9);
				 }       
				 SOC_Remain_Mark_Filter=EEPROM_Read_Byte(&g_EEPROMSPIHandle,Address_Midle_Filter);
				 Count_Mark_Filter++;         
    } 
    if(Count_Mark_Filter==0) 
    {
				Init_SOC_mAms_Filter = ((float)BATTERY_CAPACITY)*Hours_Split;
				Remain_SOC_mAms_Filter = Init_SOC_mAms_Filter/10;
				
				WriteSOC_Remian_Address_Filter = Address_SOC_Remain_Filter;
				Write_SOC_Remain(WriteSOC_Remian_Address_Filter,Remain_SOC_mAms_Filter);
				WriteSOC_Remian_Address_Filter += 9;
    } 
    else 
    {
        WriteSOC_Remian_Address_Filter=(Address_Midle_Filter-8);   
        Remain_SOC_mAms_Filter = Read_SOC_Remain(WriteSOC_Remian_Address_Filter);//��ȡʣ����������λ����.��
				Init_SOC_mAms_Filter = ((float)BATTERY_CAPACITY)*Hours_Split;
				if(Remain_SOC_mAms_Filter > Init_SOC_mAms_Filter)  //���ʣ���������ڳ�ʼ��������˵������Ѿ����ͣ�ʣ������Ϊ��ʼ����
				{
						Remain_SOC_mAms_Filter = Init_SOC_mAms_Filter;
						Write_SOC_Remain(WriteSOC_Remian_Address_Filter,Remain_SOC_mAms_Filter);
						WriteSOC_Remian_Address_Filter += 9;
				}
    }
		
		SOC_Value_Filter = (uint16_t)(((float)(Remain_SOC_mAms_Filter/Init_SOC_mAms_Filter))*1000+0.5f);
	
	if(SOC_Value_Filter>995)
	{
		Full_Capacity_Flag = 1; 
	}
		
		
		//���˲�
    //��EEPROM�ж�ȡ��������
    //��ȡʣ������
    Address_Midle_Pulse = (Address_SOC_Remain_Pulse+8);
    SOC_Remain_Mark_Pulse = EEPROM_Read_Byte(&g_EEPROMSPIHandle,Address_Midle_Pulse);
    
    while(SOC_Remain_Mark_Pulse != 0x55) 
    {
				 if(Count_Mark_Pulse>=(SOC_Storage_MAX_Num*2)) 
				 {
						 Count_Mark_Pulse=0;
						 break;
				 }       
				 if(Address_Midle_Pulse>=(Address_SOC_Remain_Pulse+(SOC_Storage_MAX_Num*9)-1)) 
				 {
							Address_Midle_Pulse= (Address_SOC_Remain_Pulse+8);
				 } 
				 else 
				 {
							Address_Midle_Pulse= (Address_Midle_Pulse+9);
				 }       
				 SOC_Remain_Mark_Pulse=EEPROM_Read_Byte(&g_EEPROMSPIHandle,Address_Midle_Pulse);
				 Count_Mark_Pulse++;         
    } 
    if(Count_Mark_Pulse==0) 
    {
				Init_SOC_mAms_Pulse = ((float)BATTERY_CAPACITY)*Hours_Split;
				Remain_SOC_mAms_Pulse = Init_SOC_mAms_Pulse/10;
			
				WriteSOC_Remian_Address_Pulse = Address_SOC_Remain_Pulse;
				Write_SOC_Remain(WriteSOC_Remian_Address_Pulse,Remain_SOC_mAms_Pulse);
				WriteSOC_Remian_Address_Pulse += 9;
    } 
    else 
    {
        WriteSOC_Remian_Address_Pulse=(Address_Midle_Pulse-8);   
        Remain_SOC_mAms_Pulse = Read_SOC_Remain(WriteSOC_Remian_Address_Pulse);//��ȡʣ����������λ����.��
				Init_SOC_mAms_Pulse = ((float)BATTERY_CAPACITY)*Hours_Split;
				if(Remain_SOC_mAms_Pulse > Init_SOC_mAms_Pulse)  //���ʣ���������ڳ�ʼ��������˵������Ѿ����ͣ�ʣ������Ϊ��ʼ����
				{
						Remain_SOC_mAms_Pulse = Init_SOC_mAms_Pulse;
						Write_SOC_Remain(WriteSOC_Remian_Address_Pulse,Remain_SOC_mAms_Pulse);
						WriteSOC_Remian_Address_Pulse += 9;
				}
    }
		
		SOC_Value_Pulse = (uint16_t)(((float)(Remain_SOC_mAms_Pulse/Init_SOC_mAms_Pulse))*1000+0.5f);
	
	if(SOC_Value_Pulse>995)
	{
		Full_Capacity_Flag = 1; 
	}
}


void Remain_SOC_mAms_Pulse_Update(void)
{
		SOC_Current_Pulse = Get_Pulse_Current();
		Pulse_SOC_Change = (SOC_Current_Pulse+SOC_Current_Pulse_Pre)/2.0f*1000/TIM3_Freq;
		Remain_SOC_mAms_Pulse_Mod += Pulse_SOC_Change;
		if((Remain_SOC_mAms_Pulse_Mod>1000)||(Remain_SOC_mAms_Pulse_Mod<-1000))
		{
				Remain_SOC_mAms_Pulse -= Remain_SOC_mAms_Pulse_Mod;				
				Remain_SOC_mAms_Pulse_Mod = 0;
				Remain_SOC_mAms_Pulse = range(Remain_SOC_mAms_Pulse, 0, Init_SOC_mAms_Pulse);
		}
		SOC_Current_Pulse_Pre = SOC_Current_Pulse;
}

void Remain_SOC_mAms_Fliter_Update(void)
{
		SOC_Current_Filter = Get_Filter_Current();
//		Filter_SOC_Change = (SOC_Current_Filter+SOC_Current_Filter_Pre)/2.0f*10;
//		Remain_SOC_mAms_Filter = Remain_SOC_mAms_Filter- Filter_SOC_Change;
		Remain_SOC_mAms_Filter -= (SOC_Current_Filter+SOC_Current_Filter_Pre)/2.0f*10;
		SOC_Current_Filter_Pre = SOC_Current_Filter;
		BMS_To_HMI.Total_Current = (s16)(SOC_Current_Filter/10+0.5f);
		Remain_SOC_mAms_Filter = range(Remain_SOC_mAms_Filter, 0, Init_SOC_mAms_Filter);
}


void SOC_PerK_Update(void)
{
		SOC_Value_Filter=(u16)((Remain_SOC_mAms_Filter/Init_SOC_mAms_Filter)*1000+0.5f);        				//��ǰSOCǧ�ֱ�
		SOC_Value_Pulse=(u16)((Remain_SOC_mAms_Pulse/Init_SOC_mAms_Pulse)*1000+0.5f);        				//��ǰSOCǧ�ֱ�
		BMS_To_HMI.Pack_SOC = SOC_Value_Filter;
		//BMS_To_HMI.Pack_SOC = SOC_Value_Pulse;
}









