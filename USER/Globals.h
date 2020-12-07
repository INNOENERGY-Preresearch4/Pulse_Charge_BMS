/*
*********************************************************************************************************
*
*                                     				ȫ�ֱ�������
*
* Filename      : _Globals.h
* Version       : V1.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*********************************************************************************************************
*/

#ifndef _Globals_H_
#define _Globals_H_

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "sys.h"
#include  "Config.h"

/*
*********************************************************************************************************
*                                             Para DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

////////////////////////////////////////////////////////
// ����ʱ�估�������
////////////////////////////////////////////////////////
extern 	u32 Run_Time;			//��������ʱ��
extern 	u8 	Flag_500ms;		//500ms��־
extern 	u8 	Flag_50ms;		//100ms��־
extern 	u8 	Flag_100ms;		//200ms��־


////////////////////////////////////////////////////////
// ���pack��ѹ���¶Ȳ������
////////////////////////////////////////////////////////
extern uint32_t  PACK_Voltage_mV;    // PACK_VoltageΪ������ӵó����ܵ�ѹ
extern uint16_t  pack_vol;         // pack_volΪ�����ܵ�ѹ
extern uint8_t   Cell_Max_Num;         //�����ѹ������
extern uint8_t   Cell_Min_Num;         //�����ѹ��С����
extern uint16_t  Max_CellVol;          //��������ѹ
extern uint16_t  Min_CellVol;          //������С��ѹ
extern s16  		 Pack_MaxTemp;          //����¶�
extern s16  		 Pack_MinTemp;          //��С�¶�
extern uint8_t   Temp_Min_Num,Temp_Max_Num;//��߼�����¶ȵ���
extern u16 cellvol[TOTAL_IC*12];		//����鵥�������ѹ
extern u16 LTC6804_GpioVol[TOTAL_IC*6];			//������¶Ȳ�����ѹ
extern s16	Pack_TempArr[TEMP_NUM];								//������¶�

/////////////////////////////////////////////////////////////////////////////////////////
// SOC����������ر���
/////////////////////////////////////////////////////////////////////////////////////////
//�˲���ļ�����ر�����ֻ��Ϊ�Ա�������
volatile extern uint16_t SOC_Value_Filter;                //��ǰSOCǧ�ֱ�
extern float Init_SOC_mAms_Filter;                  //������ʼ����,����.��
volatile extern float Remain_SOC_mAms_Filter;      //�����ʣ������,����.��
volatile extern float  Cur_Ref_V_Filter;           //��������У׼ֵ
volatile extern float  Cur_Measure_V_Filter;
volatile extern float 	SOC_Current_Filter;    			// ����SOC����ĵ���ֵ
volatile extern float 	SOC_Current_Filter_Pre;    			// ����SOC����ĵ���ֵ
extern uint32_t	WriteSOC_Remian_Address_Filter ;
#define   Cur_Filter   2

//û�˲��ļ�����ر��������뱣��ֵʹ��
volatile extern uint16_t  SOC_Value_Pulse;        				//��ǰSOCǧ�ֱ�
extern float  Init_SOC_mAms_Pulse;        	//������ʼ����,����.��
volatile extern float  Remain_SOC_mAms_Pulse;  			//�����ʣ������,����.��
volatile extern float   Cur_Ref_V_Pulse; 						//����������׼ֵ
volatile extern float   Cur_Measure_V_Pulse;
volatile extern float	 SOC_Current_Pulse;							//������SOC����ĵ���ֵ��mA
volatile extern float 	 SOC_Current_Pulse_Pre;    			// ����SOC����ĵ���ֵ
extern uint32_t	 WriteSOC_Remian_Address_Pulse;//û�˲���SOC�洢�׵�ַ
#define   Cur_Pulse   1

extern float SOC_TIM_Base;//SOC����ʱ���׼
extern u8 Full_Capacity_Flag;//�Ƿ������1Ϊ������0Ϊδ����
extern u8 Full_Capacity_Num;//�Ƿ�����������

/////////////////////////////////////////////////////////////////////////////////////////
// ���״̬
/////////////////////////////////////////////////////////////////////////////////////////

extern  u8 	OverCharge;

/////////////////////////////////////////////////////////////////////////////////////////
// ����״̬
/////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t     need_balance;         //�Ƿ���Ҫ����
extern uint8_t  	 blance_stop_cellcharge;

/////////////////////////////////////////////////////////////////////////////////////////
// ���ϱ�ʶ
/////////////////////////////////////////////////////////////////////////////////////////
extern uint8_t Host_COMM_error_flag;//����λ��ͨѶ���ϱ�ʶ��5s��û�յ���λ������Ϣ
extern uint8_t Host_COMM_error_cont;//ͨѶ���ϼ���
extern uint8_t HOST_COMM_ON;

extern uint8_t temp_error_cont; //�¶Ȳ����������
extern uint8_t vol_error_cont; //��ѹ�����������
extern uint8_t cur_ad_count;   //���������������

extern uint8_t tempover_count;    //�����ŵ���ȱ�����������
extern uint8_t  templow_count;          //�������䱨����������

extern uint8_t  overpackvoltage_count;  //������ѹ���ϱ����������� 
extern uint8_t  lowpackvoltage_count;   //�����Ƿѹ���ϱ�����������

extern uint8_t  dcurrent_count;         //�ŵ����������ϱ�����������  
extern uint8_t  ccurrent_count;         //���������󱨾���������

extern uint8_t  overcellvoltage_count;      //�����ѹ���ϱ�����������
extern uint8_t  lowcellvoltage_count;       //����Ƿѹ���ϱ�����������

extern uint8_t  cell_diff_blance;       //���岻ƽ�ⱨ������
extern uint8_t  cell_diff_blance_free;  //���岻ƽ�ⱨ���������
extern uint8_t  unblance_count;         //���岻ƽ����ϱ�����������

extern uint8_t  cutoff_tempover_count;    //�����ŵ���ȱ�����������
extern uint8_t  cutoff_overpackvoltage_count;  //������ѹ���ϱ����������� 
extern uint8_t  cutoff_lowpackvoltage_count;   //�����Ƿѹ���ϱ�����������
extern uint8_t  cutoff_overcellvoltage_count;      //�����ѹ���ϱ�����������
extern uint8_t  cutoff_lowcellvoltage_count;       //����Ƿѹ���ϱ�����������
extern uint8_t  cutoff_ccurrent_count;         //���������󱨾���������
extern uint8_t  cutoff_dcurrent_count;         //�ŵ����������ϱ����������� 


/////////////////////////////////////////////////////////////////////////////////////////
// HMIָ�����
/////////////////////////////////////////////////////////////////////////////////////////
extern 	 u8 	CMD_Battery_Mode;//0x01Ϊ��磬0x02Ϊ�ŵ磬����Ϊ��
extern 	 s32 	Battery_Mode_Time;//ָ�����õ�ʱ�����ԣ���sΪ��λ
#define  CMD_Charge      (0x01)      //���
#define  CMD_Discharge   (0x02)      //�ŵ�

/////////////////////////////////////////////////////////////////////////////////////////
// �̵������
/////////////////////////////////////////////////////////////////////////////////////////
extern 		u8 Relay_Flag;//
#define  	Flag_ON      (0x01)      //�̵�������
#define  	Flag_OFF     (0x00)      //�̵����Ͽ�

/*
*********************************************************************************************************
*                                           	STRUCTS
*********************************************************************************************************
*/

#define BMS_HMI_SyncHeader1 		0xEB
#define BMS_HMI_SyncHeader2 		0x90
#define BMS_To_HMI_Address			0x01
#define BMS_To_HMI_Message_ID		0x10
#define HMI_To_BMS_Address			0x02
#define HMI_To_BMS_Message_ID_CMD			0x20
#define HMI_To_BMS_Message_ID_NULL		0x30
#define HMI_To_BMS_CMD_Charge					0x01
#define HMI_To_BMS_CMD_Discharge			0x02
#define BMS_To_HMI_Num			50
#define HMI_To_BMS_Num			16

//��HMI��ĻͨѶ
#pragma  pack(push,1)   // 61
typedef struct {
	u8    SyncHeader1;  //֡ͬ����1  0xEB
	u8    SyncHeader2;  //֡ͬ����2  0x90
	u8    Address;			//��ַ  0x01
	u8    Message_ID; 	//֡ʶ����  0x10
	
	u8    Battery_Status; //����鹤��״̬
	u8    Pack_Alarms[3];	//����������Ϣ
	u16   Total_Voltage; 	//������ܵ�ѹ
	s16 	Total_Current;  //������ܵ���
	
	u16 	Pack_SOC; 					//�����ʣ������
	u16 	Cell_Voltage[12]; 	//��ص����ѹ
	u16 	CycledCount_Remain; //���ʣ��ʹ�ô���
	u16 	Days_Remain; 				//���ʣ����������
	s16 	Pack_Temp; 			//������¶�
	
	u8 	  Rev[4]; //Ԥ��
	u8    Message_Num; //֡����
	u8	  SumCheck;
} BMS_To_HMI_Typedef;
#pragma pack(pop)

//��HMI��ĻͨѶ
#pragma  pack(push,1)   // 61
typedef struct {
	u8    SyncHeader1;  //֡ͬ����1  0xEB
	u8    SyncHeader2;  //֡ͬ����2  0x90
	u8    Address;			//��ַ  0x02
	u8    Message_ID; 	//֡ʶ����  0x20=��ָ�0x30=��ָ��
	
	u8    Cmd_Mode; 		//0x01=��磬0x02=�ŵ磬����=�޶���
	u8    Cmd_ParamL;		//LSB=1min  ��Χ0min~1440min
	u8    Cmd_ParamH; 	//LSB=1min  ��Χ0min~1440min
	u8 	  Cmd_STOP; 			//λ0��1Ϊ��ͣ  0Ϊ�޲���
	
	u8 	  Rev[6]; //Ԥ��
	u8    Message_Num; //֡����
	u8	  SumCheck;
} HMI_To_BMS_Typedef;
#pragma pack(pop)

/*
*********************************************************************************************************
*                                           	EXTERN  STRUCTS
*********************************************************************************************************
*/

extern 	BMS_To_HMI_Typedef BMS_To_HMI;
extern 	HMI_To_BMS_Typedef HMI_To_BMS;


#endif


