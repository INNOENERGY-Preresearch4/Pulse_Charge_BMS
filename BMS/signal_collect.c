/* Including needed modules to compile this module/procedure */
#include "includes.h"
u32 voltage_pre=0;
s32  current_pre = 0;
s32 cur_ref=VOLTAGE_REF_25;
unsigned int TemperatureTable[181] =   //10K B=3950   厂家配置
{
65530, //-55
65530, //-54
65530, //-53
65530, //-52
65530, //-51
65530, //-50
65530, //-49
65530, //-48
65530, //-47
62867, //-46
58253, //-45
54014, //-44
50117, //-43
46530, //-42
43223, //-41
33156, //-40
31065, 
29117, 
27301, 
25608, 
24028,  //-35
22554, 
21178, 
19894, 
18695, 
17575,  //-30
16529,
15552,
14638,
13784,
12985,  //-25
12237,
11537,
10881,
10267,
9690,  //-20
9149,
8641,
8163,
7715,
7293,   //-15
6897,
6524,
6173,
5845,
5532,    //-10
5238,
4963,
4702,
4458,
4227,   //-5
4009,
3804,
3610,
3428,
3256,   //0
3093,
2940,
2795,
2658,
2529,   //5
2407,
2292,
2183,
2080,
1982,    //10
1990,
1802,
1720,
1641,
1567,    //15
1496,
1429,
1365,
1305,
1248,     //20
1193,
1141,
1092,
1045,
1000,       //25
957,
917,
878,
841,
806,        //30
773,
740,
710,
681,
654,         //35
627,
601,
577,
555,
532,          //40
511,
491,
471,
454,
436,        //45
420,
403,
387,
372,
359,         //50
345,         //继续
332,
320,
308,
297,        //55
286,
276,
266,
257,
247,        //60
238,
230,
222,
214,
207,        //65
199,
193,
186,
180,
174,       //70
168,
162,
157,
152,
147,       //75
142,
137,
133,
128,
124,       //80
120,
117,
113,
109,
106,       //85
103,
99,
96,
93,
90,         //90
88,
85,
83,
80,
78 ,        //95
75 ,
73 ,
71 ,
69 ,
67 ,       //100
65 ,
63 ,
61 ,
59 ,
58 ,    //105
56 ,
54 ,
53 ,
51 ,
50 ,    //110
48 ,
47 ,
46 ,
45 ,
43 ,    //115
42 ,
41 ,
40 ,
39 ,
38 ,      //120
37 ,
36 ,
35 ,
34 ,
33       //125
}; 
//总电流采集
s32 get_current(u16 AD_value)
{
    s32  current = 0;    //电流值,mA
    s32  voltage_A = 0;    //采集电压值,mV 
    voltage_A = (AD_value*VOLTAGE_REF)/4096;   
    //电流传感器量程-200-200A，正为加，负为减 2.5V+/-1V
    if((voltage_A>((s32)cur_ref+10))||(((voltage_A+10)<(s32)cur_ref))) 
    {
         current=(s32)((voltage_A-cur_ref)*(300));
        if((current<1000)&&(current>-1000)) 
        {
            current=-600; 
        } 
    } 
    else 
    {
        current=-600;
    }
    current=(current_pre*3+current)/4; 
    current_pre=current;                                              
    return current; 
} 
//总电流采集
s16 get_current_ref(u16 AD_value)
{
    s32  voltage_A = 0;    //采集电压值,mV 
    
    voltage_A = AD_value;
    if((voltage_A>(VOLTAGE_REF_25+30))||((voltage_A+30)<VOLTAGE_REF_25)) 
    {
      cur_ref=VOLTAGE_REF_25;
    }else 
    {  
       cur_ref=voltage_A;
    }
    return voltage_A; 
}

//读取温度
//channel 0-AD_CHANLE_NUM
s8 get_AD_temp(u8 channel)
{
    u16 NTCres;
    u32 temp_v=0;
  	s8 temp = 0;
	temp_v=(ADC_ConvertValue2[channel]*VOLTAGE_REF)/4096;
    //根据电压值计算温度值    
    if((temp_v>(VOLTAGE_REF-50))||(temp_v<30)) //断线满偏
    {
       temp=INVALID_TEMP;
    } 
    else 
    {
       NTCres = (unsigned int)((((unsigned long)temp_v)*R22)/(VOLTAGE_REF_2-temp_v));
       temp = calculate_wendu(NTCres);
    }    
    return temp; 
}
//根据温度传感器电阻计算温度
s8 calculate_wendu(u16 resistor) 
{
  	int n = sizeof(TemperatureTable)/sizeof(unsigned int);
  	int i = 0;
  	signed char temp = 0;  	
  	if(resistor < TemperatureTable[n-1])
  	{ 
  	    return MAX_TEMP;
  	}  	
  	if(resistor > TemperatureTable[0])
  	{
  	    return MIN_TEMP;
  	}
  	for(i = 0;i < n;i++)
  	{
    		if(TemperatureTable[i] == resistor)
    		{
      			temp = (MIN_TEMP + i);
      			return temp;
    		}
    		else
    		{
      			if(TemperatureTable[i] > resistor)
      			{
        				continue;
      			}
      			else
      			{
      			    temp = (signed char)(MIN_TEMP + i - ((float)(resistor - TemperatureTable[i]))/(TemperatureTable[i - 1] - TemperatureTable[i]));
                return temp;
      			}
    		}
  	}
	return 0;
}

//获取差值的绝对值
u8 Getabsvalue(s8 value1,s8 value2)
{
    if(value2>=value1)
        return (value2-value1);
    else
        return (value1-value2);
}
//从数组中求电压最小值以及编号
u16 CellMinValue(u16 data[],u16 n,u8*num)
{
	u16 i = 0;
	u16 minvalue = 0;
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
//从数组中求电压最大值
u16 CellMaxValue(u16 data[],u16 n,u8*num)
{
	u16 i = 0;
	u16 maxvalue = 0;
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
//从数组中求温度最小值以及编号
s8 TempMinValue(s8 data[],u8 n,u8*num)
{
	u16 i = 0;
	s8 minvalue = 125;
	*num=1;
	for(i = 0;i < n;i++)
	{		
	  if(data[i]!=INVALID_TEMP) 
	  { 
    	  if(minvalue > data[i])
    	  {
    			minvalue = data[i];
    			*num=(i+1);		
    		}
	  }					
	}
  return minvalue;	
}
//从数组中求温度最大值
s8 TempMaxValue(s8 data[],u8 n,u8*num)
{
	u16 i = 0;
	s8 maxvalue = -125;
  *num=1;
	for(i = 0;i < n;i++)
	{	
	  if(data[i]!=INVALID_TEMP) 
	  { 	
		  if(maxvalue < data[i])
		  {
				maxvalue = data[i];
			*num=(i+1);		
		  }
  	  }							
	}
  return maxvalue;	
}
//读取精密电压电流数据
void hprec_read(s32 *cur,u16 *vol)
{
	s32 read_value[4]={0};
    read_value[0] = AD7787_Read_Value(5,0x01);//1 AIN1(+)-AIN1(-);2 AIN2;3 AIN1(-)-AIN1(-); 4 VDD Monitor
	read_value[1] = AD7787_Read_Value(5,0x02);
	read_value[2] = AD7787_Read_Value(5,0x03);
	read_value[3] = AD7787_Read_Value(5,0x04);
	*cur = read_value[0];
	*vol = read_value[1];
}


///////////////////////////////////////////////////////////////////////////////

