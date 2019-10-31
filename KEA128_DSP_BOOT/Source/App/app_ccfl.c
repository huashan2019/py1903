/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : APP_ccfl.c  ---   背光
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

SCH_U8 PowerOpenScreenTimer;


/********************************************************************************
**  Function	: CCFL_POWER
**  Author		: 
**  Created on	: 20170428
**  Description	:
**  Return		: 
********************************************************************************/
void CCFL_Power_Init(void)
{///===
	GPIO_PinInit(GPIO_CCFL_POWER_CTL, GPIO_PinOutput);
}
void CCFL_Power_Ctl(SCH_BOOL OnOff)
{
	SCH_U16 u16ChannelValue;
	if(OnOff == ON)
	{
		///TurnOn_CCFL_POWER;
		Set_CCFL_POWER_Flag;
		u16ChannelValue = 1000+200*GetBrightness();
	}
	else
	{
		///TurnOff_CCFL_POWER;
		Clr_CCFL_POWER_Flag;
		u16ChannelValue = 0;
	}
	FTM_SetChannelValue(FTM0, FTM_CHANNEL_CHANNEL0, u16ChannelValue);///占空比  =  5000/u16ChannelValue
}
void CcflInit(void)
{
	Ill_Brightness = DEF_ILL_BRIGHTNESS;
	USER_Brightness = DEF_USER_BRIGHTNESS;
	PowerOpenScreenTimer = 0x00;
}
SCH_U8 GetBrightness(void)
{
	return (Get_ILL_Flag) ? Ill_Brightness : USER_Brightness;
}
void SetBrightness(SCH_U8 data)
{
	if(data > MAX_BRIGHTNESS/*||data < MIN_BRIGHTNESS*/)
		return;
	if(Get_ILL_Flag)
		Ill_Brightness = data;
	else
		USER_Brightness = data;
	CCFL_Power_Ctl(ON);
}
void SetCCFL_DelayTime(SCH_U8 time)
{
	PowerOpenScreenTimer = GetMax(PowerOpenScreenTimer,time);
}
/********************************************************************************
**  Function      : TASK_CCFL_Pro
**  Author        : 
**  Created on    : 
**  Description   : 16MS执行一次
**  Return        : 
********************************************************************************/
void TASK_CCFL_Pro(void)
{
	if(PowerOpenScreenTimer)
	{
		PowerOpenScreenTimer--;
		if(PowerOpenScreenTimer==0)
		{
			if(Get_ACC_Flag
				&&SysPower.nPowerState==POWER_NORMAL_RUN
				&&Get_OsStartOk)
			{
				CCFL_Power_Ctl(ON);
			}
		}
    }
}


