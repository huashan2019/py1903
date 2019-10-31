/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_mute.c  ---   æ≤“Ù
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

MUTE_STATE Mute_State;
void AudioMute(MUTE_T MUTE_Type)
{
	if(MUTE_Type==SOFTON)
	{
		AdjustVolumeStepper(SLOW_DOWN);
	}
	else if(MUTE_Type== SOFTOFF)
	{
		AdjustVolumeStepper(SLOW_UP);
	}	
	else if(MUTE_Type== HARDON)
	{
		MUTE_Ctl(ON);
	}
	else if(MUTE_Type==HARDOFF)
	{
		MUTE_Ctl(OFF);
	}
}



void TASK_Mute_Pro(void)
{
	if(Get_SysPower_Flag==OFF||Get_ACC_Flag==OFF)
	{
		Mute_State=MUTE_IDLE_STATE;
		return;
	}
	switch(Mute_State)
	{
		case MUTE_IDLE_STATE:
			if(Get_SYS_MUTE)
				AudioMute(HARDON);
			else if(Get_REV_Flag&&Get_AppStartOk&&Get_BT_GATE_Flag==OFF)
				Mute_State= MUTE_REVERSING;
			break;
		case MUTE_UNMUTE_STATE:
			if(Get_SYS_MUTE)
			{
				AudioMute(SOFTON);
				AudioMute(HARDON);
			}
			else if(SysPower.nPowerState==POWER_NORMAL_RUN&&Get_VolErr_Flag==NORMAL)    
			{
				if(SysChannel.nChannelState==CHANNEL_IDLE)
				{
					if(Get_REV_Flag==OFF||Get_BT_GATE_Flag==ON)
					{
						if(Get_BT_GATE_Flag && Get_BT_MUTE || Get_NAVI_GATE_Flag && Get_NAV_MUTE || !Get_BT_GATE_Flag && !Get_NAVI_GATE_Flag && Get_MED_MUTE )
							AudioMute(SOFTON);
						else	
						{
							AudioMute(SOFTOFF);
							AudioMute(HARDOFF);
						}
					}
				}
			}
			Mute_State=MUTE_IDLE_STATE;
			break; 
 		case MUTE_REVERSING:
			AudioMute(SOFTON);
			AudioMute(HARDON);
			Mute_State=WAIT_REVERSE_END;
			break;
		case WAIT_REVERSE_END:
			if(Get_REV_Flag==OFF)
			{
				AudioMute(HARDOFF);
				AudioMute(SOFTOFF);
				Mute_State=MUTE_IDLE_STATE;
			}
			break;      
		default:
			break;
	}
}

























